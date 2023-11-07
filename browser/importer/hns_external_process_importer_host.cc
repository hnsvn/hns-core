/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/importer/hns_external_process_importer_host.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/files/file_util.h"
#include "base/functional/bind.h"
#include "base/json/json_reader.h"
#include "base/task/thread_pool.h"
#include "hns/browser/importer/hns_importer_p3a.h"
#include "hns/common/importer/chrome_importer_utils.h"
#include "hns/common/importer/importer_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "hns/browser/importer/extensions_import_helpers.h"
#include "chrome/browser/extensions/webstore_install_with_prompt.h"
#include "chrome/common/extensions/webstore_install_result.h"
#include "extensions/browser/extension_file_task_runner.h"
#endif

namespace {

#if BUILDFLAG(ENABLE_EXTENSIONS)

// Silent installer via websotre w/o any prompt or bubble.
class WebstoreInstallerForImporting
    : public extensions::WebstoreInstallWithPrompt {
 public:
  using WebstoreInstallWithPrompt::WebstoreInstallWithPrompt;

 private:
  ~WebstoreInstallerForImporting() override = default;

  std::unique_ptr<ExtensionInstallPrompt::Prompt>
      CreateInstallPrompt() const override {
    return nullptr;
  }
  bool ShouldShowPostInstallUI() const override { return false; }
};
#endif

}  // namespace

HnsExternalProcessImporterHost::HnsExternalProcessImporterHost()
    : weak_ptr_factory_(this) {}
HnsExternalProcessImporterHost::~HnsExternalProcessImporterHost() = default;

#if BUILDFLAG(ENABLE_EXTENSIONS)
void HnsExternalProcessImporterHost::LaunchExtensionsImport() {
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE,
      {base::MayBlock(), base::TaskPriority::USER_VISIBLE,
       base::TaskShutdownBehavior::CONTINUE_ON_SHUTDOWN},
      base::BindOnce(&GetImportableChromeExtensionsList,
                     source_profile_.source_path),
      base::BindOnce(
          &HnsExternalProcessImporterHost::OnGetChromeExtensionsList,
          weak_ptr_factory_.GetWeakPtr()));
}

void HnsExternalProcessImporterHost::OnExtensionInstalled(
    const std::string& extension_id,
    bool success,
    const std::string& error,
    extensions::webstore_install::Result result) {
  if (success) {
    return;
  }
  VLOG(1) << "Extension " << extension_id << " import failed";
  extensions::GetExtensionFileTaskRunner()->PostTaskAndReply(
      FROM_HERE,
      base::BindOnce(&hns::RemoveExtensionsSettings, profile_->GetPath(),
                     extension_id),
      base::BindOnce(
          &HnsExternalProcessImporterHost::OnExtensionSettingsRemoved,
          weak_ptr_factory_.GetWeakPtr(), extension_id));
}

void HnsExternalProcessImporterHost::OnExtensionSettingsRemoved(
    const std::string& extension_id) {
  if (settings_removed_callback_for_testing_)
    settings_removed_callback_for_testing_.Run();
}

void HnsExternalProcessImporterHost::SetSettingsRemovedCallbackForTesting(
    base::RepeatingClosure callback) {
  settings_removed_callback_for_testing_ = std::move(callback);
}

void HnsExternalProcessImporterHost::InstallExtension(const std::string& id) {
  if (install_extension_callback_for_testing_) {
    install_extension_callback_for_testing_.Run(id);
    return;
  }

  scoped_refptr<WebstoreInstallerForImporting> installer =
      new WebstoreInstallerForImporting(
          id, profile_,
          base::BindOnce(
              &HnsExternalProcessImporterHost::OnExtensionInstalled,
              weak_ptr_factory_.GetWeakPtr(), id));
  installer->BeginInstall();
}

void HnsExternalProcessImporterHost::ImportExtensions(
    std::vector<std::string> ids) {
  for (const auto& id : ids) {
    InstallExtension(id);
  }

  if (!ids.empty() && observer_)
    observer_->ImportItemEnded(importer::EXTENSIONS);

  ExternalProcessImporterHost::NotifyImportEnded();
}

void HnsExternalProcessImporterHost::OnGetChromeExtensionsList(
    absl::optional<std::vector<std::string>> extensions_list) {
  if (!extensions_list.has_value()) {
    ExternalProcessImporterHost::NotifyImportEnded();
    return;
  }
  const auto ids = extensions_list.value();
  if (ids.empty()) {
    ExternalProcessImporterHost::NotifyImportEnded();
    return;
  }
  extensions::GetExtensionFileTaskRunner()->PostTaskAndReply(
      FROM_HERE,
      base::BindOnce(&hns::ImportStorages, source_profile_.source_path,
                     profile_->GetPath(), ids),
      base::BindOnce(&HnsExternalProcessImporterHost::ImportExtensions,
                     weak_ptr_factory_.GetWeakPtr(), ids));
}

void HnsExternalProcessImporterHost::NotifyImportEnded() {
  if (!cancelled_)
    RecordImporterP3A(source_profile_.importer_type);

  // If user chooses extension importing, start importing extensions.
  // and NotifyImportEnded() will be called from OnGetChromeExtensionsList().
  // Handling extensions importing after finishing all other properties makes
  // logic simpler.
  // Don't import if cancelled.
  if (!cancelled_ && (items_ & importer::EXTENSIONS)) {
    LaunchExtensionsImport();
    return;
  }

  // Otherwise, notifying here and importing is finished.
  ExternalProcessImporterHost::NotifyImportEnded();
}

void HnsExternalProcessImporterHost::LaunchImportIfReady() {
  if (do_not_launch_import_for_testing_)
    return;
  ExternalProcessImporterHost::LaunchImportIfReady();
}

void HnsExternalProcessImporterHost::DoNotLaunchImportForTesting() {
  do_not_launch_import_for_testing_ = true;
}

void HnsExternalProcessImporterHost::SetInstallExtensionCallbackForTesting(
    MockedInstallCallback callback) {
  install_extension_callback_for_testing_ = std::move(callback);
}

void HnsExternalProcessImporterHost::NotifyImportEndedForTesting() {
  ExternalProcessImporterHost::NotifyImportEnded();
}

importer::ImporterProgressObserver*
HnsExternalProcessImporterHost::GetObserverForTesting() {
  return observer_;
}

#endif
