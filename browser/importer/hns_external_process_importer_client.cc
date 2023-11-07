/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/importer/hns_external_process_importer_client.h"

#include <utility>

#include "base/functional/bind.h"
#include "hns/browser/importer/hns_in_process_importer_bridge.h"
#include "chrome/grit/generated_resources.h"
#include "content/public/browser/service_process_host.h"

namespace {
bool ShouldUseHnsImporter(importer::ImporterType type) {
  switch (type) {
    case importer::TYPE_CHROME:
    case importer::TYPE_EDGE_CHROMIUM:
    case importer::TYPE_VIVALDI:
    case importer::TYPE_OPERA:
    case importer::TYPE_YANDEX:
    case importer::TYPE_WHALE:
      return true;
    default:
      return false;
  }
}
}  // namespace

template <>
inline sandbox::mojom::Sandbox
content::GetServiceSandboxType<hns::mojom::ProfileImport>() {
  return sandbox::mojom::Sandbox::kNoSandbox;
}

HnsExternalProcessImporterClient::HnsExternalProcessImporterClient(
    base::WeakPtr<ExternalProcessImporterHost> importer_host,
    const importer::SourceProfile& source_profile,
    uint16_t items,
    InProcessImporterBridge* bridge)
    : ExternalProcessImporterClient(
          importer_host, source_profile, items, bridge) {}

HnsExternalProcessImporterClient::
    ~HnsExternalProcessImporterClient() = default;

void HnsExternalProcessImporterClient::Start() {
  if (!ShouldUseHnsImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::Start();
    return;
  }

  AddRef();  // balanced in Cleanup.

  auto options = content::ServiceProcessHost::Options()
                     .WithDisplayName(IDS_UTILITY_PROCESS_PROFILE_IMPORTER_NAME)
                     .Pass();
  content::ServiceProcessHost::Launch(
      hns_profile_import_.BindNewPipeAndPassReceiver(), std::move(options));

  hns_profile_import_.set_disconnect_handler(
      base::BindOnce(&ExternalProcessImporterClient::OnProcessCrashed, this));

  base::flat_map<uint32_t, std::string> localized_strings;
  hns_profile_import_->StartImport(
      source_profile_, items_, localized_strings,
      receiver_.BindNewPipeAndPassRemote(),
      hns_receiver_.BindNewPipeAndPassRemote());
}

void HnsExternalProcessImporterClient::Cancel() {
  if (!ShouldUseHnsImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::Cancel();
    return;
  }

  if (cancelled_)
    return;

  cancelled_ = true;
  hns_profile_import_->CancelImport();
  CloseMojoHandles();
  Release();
}

void HnsExternalProcessImporterClient::CloseMojoHandles() {
  if (!ShouldUseHnsImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::CloseMojoHandles();
    return;
  }

  hns_profile_import_.reset();
  hns_receiver_.reset();
  receiver_.reset();
}

void HnsExternalProcessImporterClient::OnImportItemFinished(
    importer::ImportItem import_item) {
  if (!ShouldUseHnsImporter(source_profile_.importer_type)) {
    ExternalProcessImporterClient::OnImportItemFinished(import_item);
    return;
  }

  if (cancelled_)
    return;

  bridge_->NotifyItemEnded(import_item);
  hns_profile_import_->ReportImportItemFinished(import_item);
}

void HnsExternalProcessImporterClient::OnCreditCardImportReady(
    const std::u16string& name_on_card,
    const std::u16string& expiration_month,
    const std::u16string& expiration_year,
    const std::u16string& decrypted_card_number,
    const std::string& origin) {
  if (cancelled_)
    return;

  static_cast<HnsInProcessImporterBridge*>(
      bridge_.get())->SetCreditCard(name_on_card,
                                    expiration_month,
                                    expiration_year,
                                    decrypted_card_number,
                                    origin);
}
