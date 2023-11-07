/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/settings/hns_default_extensions_handler.h"

#include <memory>
#include <string>

#include "base/functional/bind.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "hns/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/browser/hns_wallet/tx_service_factory.h"
#include "hns/browser/extensions/hns_component_loader.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/hns_wallet/browser/tx_service.h"
#include "hns/components/hns_webtorrent/grit/hns_webtorrent_resources.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/decentralized_dns/core/constants.h"
#include "hns/components/decentralized_dns/core/utils.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/l10n/common/localization_util.h"
#include "chrome/browser/about_flags.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/extensions/component_loader.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/extensions/webstore_install_with_prompt.h"
#include "chrome/browser/lifetime/application_lifetime.h"
#include "chrome/browser/media/router/media_router_feature.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/chrome_select_file_policy.h"
#include "chrome/common/chrome_switches.h"
#include "chrome/common/pref_names.h"
#include "components/flags_ui/flags_ui_constants.h"
#include "components/flags_ui/pref_service_flags_storage.h"
#include "components/grit/hns_components_strings.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/constants.h"
#include "extensions/common/feature_switch.h"

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_constants.h"
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
#include "hns/browser/widevine/widevine_utils.h"
#endif

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/browser/ipfs/ipfs_service_factory.h"
#include "hns/components/ipfs/ipfs_service.h"
#include "hns/components/ipfs/keys/ipns_keys_manager.h"
#include "hns/components/ipfs/pref_names.h"
#endif

using decentralized_dns::EnsOffchainResolveMethod;
using decentralized_dns::ResolveMethodTypes;

namespace {

template <typename T>
base::Value::Dict MakeSelectValue(T value, const std::u16string& name) {
  base::Value::Dict item;
  item.Set("value", base::Value(static_cast<int>(value)));
  item.Set("name", base::Value(name));
  return item;
}

base::Value::List GetResolveMethodList() {
  base::Value::List list;
  list.Append(MakeSelectValue(ResolveMethodTypes::ASK,
                              hns_l10n::GetLocalizedResourceUTF16String(
                                  IDS_DECENTRALIZED_DNS_RESOLVE_OPTION_ASK)));
  list.Append(
      MakeSelectValue(ResolveMethodTypes::DISABLED,
                      hns_l10n::GetLocalizedResourceUTF16String(
                          IDS_DECENTRALIZED_DNS_RESOLVE_OPTION_DISABLED)));
  list.Append(
      MakeSelectValue(ResolveMethodTypes::ENABLED,
                      hns_l10n::GetLocalizedResourceUTF16String(
                          IDS_DECENTRALIZED_DNS_RESOLVE_OPTION_ENABLED)));

  return list;
}

base::Value::List GetEnsOffchainResolveMethodList() {
  base::Value::List list;
  list.Append(MakeSelectValue(
      EnsOffchainResolveMethod::kAsk,
      hns_l10n::GetLocalizedResourceUTF16String(
          IDS_DECENTRALIZED_DNS_ENS_OFFCHAIN_RESOLVE_OPTION_ASK)));
  list.Append(MakeSelectValue(
      EnsOffchainResolveMethod::kDisabled,
      hns_l10n::GetLocalizedResourceUTF16String(
          IDS_DECENTRALIZED_DNS_ENS_OFFCHAIN_RESOLVE_OPTION_DISABLED)));
  list.Append(MakeSelectValue(
      EnsOffchainResolveMethod::kEnabled,
      hns_l10n::GetLocalizedResourceUTF16String(
          IDS_DECENTRALIZED_DNS_ENS_OFFCHAIN_RESOLVE_OPTION_ENABLED)));

  return list;
}
}  // namespace

HnsDefaultExtensionsHandler::HnsDefaultExtensionsHandler()
    : weak_ptr_factory_(this) {
#if BUILDFLAG(ENABLE_WIDEVINE)
  was_widevine_enabled_ = IsWidevineOptedIn();
#endif
}

HnsDefaultExtensionsHandler::~HnsDefaultExtensionsHandler() = default;

void HnsDefaultExtensionsHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());
#if BUILDFLAG(ENABLE_IPFS)
  ipfs::IpfsService* service =
      ipfs::IpfsServiceFactory::GetForContext(profile_);
  if (service) {
    ipfs_service_observer_.Observe(service);
  }
  web_ui()->RegisterMessageCallback(
      "notifyIpfsNodeStatus",
      base::BindRepeating(&HnsDefaultExtensionsHandler::CheckIpfsNodeStatus,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setIPFSStorageMax",
      base::BindRepeating(&HnsDefaultExtensionsHandler::SetIPFSStorageMax,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "importIpnsKey",
      base::BindRepeating(&HnsDefaultExtensionsHandler::ImportIpnsKey,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "launchIPFSService",
      base::BindRepeating(&HnsDefaultExtensionsHandler::LaunchIPFSService,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "exportIPNSKey",
      base::BindRepeating(&HnsDefaultExtensionsHandler::ExportIPNSKey,
                          base::Unretained(this)));
#endif
  web_ui()->RegisterMessageCallback(
      "resetWallet",
      base::BindRepeating(&HnsDefaultExtensionsHandler::ResetWallet,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "resetTransactionInfo",
      base::BindRepeating(&HnsDefaultExtensionsHandler::ResetTransactionInfo,
                          base::Unretained(this)));

  web_ui()->RegisterMessageCallback(
      "setWebTorrentEnabled",
      base::BindRepeating(&HnsDefaultExtensionsHandler::SetWebTorrentEnabled,
                          base::Unretained(this)));
#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
  web_ui()->RegisterMessageCallback(
      "setHnsWalletEnabled",
      base::BindRepeating(&HnsDefaultExtensionsHandler::SetHnsWalletEnabled,
                          base::Unretained(this)));
#endif
  web_ui()->RegisterMessageCallback(
      "setHangoutsEnabled",
      base::BindRepeating(&HnsDefaultExtensionsHandler::SetHangoutsEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setIPFSCompanionEnabled",
      base::BindRepeating(
          &HnsDefaultExtensionsHandler::SetIPFSCompanionEnabled,
          base::Unretained(this)));
  // TODO(petemill): If anything outside this handler is responsible for causing
  // restart-neccessary actions, then this should be moved to a generic handler
  // and the flag should be moved to somewhere more static / singleton-like.
  web_ui()->RegisterMessageCallback(
      "getRestartNeeded",
      base::BindRepeating(&HnsDefaultExtensionsHandler::GetRestartNeeded,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setWidevineEnabled",
      base::BindRepeating(&HnsDefaultExtensionsHandler::SetWidevineEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isWidevineEnabled",
      base::BindRepeating(&HnsDefaultExtensionsHandler::IsWidevineEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getDecentralizedDnsResolveMethodList",
      base::BindRepeating(
          &HnsDefaultExtensionsHandler::GetDecentralizedDnsResolveMethodList,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getEnsOffchainResolveMethodList",
      base::BindRepeating(
          &HnsDefaultExtensionsHandler::GetEnsOffchainResolveMethodList,
          base::Unretained(this)));

  // Can't call this in ctor because it needs to access web_ui().
  InitializePrefCallbacks();
}

void HnsDefaultExtensionsHandler::InitializePrefCallbacks() {
#if BUILDFLAG(ENABLE_WIDEVINE)
  local_state_change_registrar_.Init(g_browser_process->local_state());
  local_state_change_registrar_.Add(
      kWidevineOptedIn,
      base::BindRepeating(
          &HnsDefaultExtensionsHandler::OnWidevineEnabledChanged,
          base::Unretained(this)));
#endif
  pref_change_registrar_.Init(profile_->GetPrefs());
  pref_change_registrar_.Add(
      kDefaultEthereumWallet,
      base::BindRepeating(&HnsDefaultExtensionsHandler::OnWalletTypeChanged,
                          base::Unretained(this)));
}

bool HnsDefaultExtensionsHandler::IsRestartNeeded() {
#if BUILDFLAG(ENABLE_WIDEVINE)
  if (was_widevine_enabled_ != IsWidevineOptedIn())
    return true;
#endif

  return false;
}

void HnsDefaultExtensionsHandler::GetRestartNeeded(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);

  AllowJavascript();
  ResolveJavascriptCallback(args[0], base::Value(IsRestartNeeded()));
}

void HnsDefaultExtensionsHandler::ResetWallet(const base::Value::List& args) {
  auto* hns_wallet_service =
      hns_wallet::HnsWalletServiceFactory::GetServiceForContext(profile_);
  if (hns_wallet_service)
    hns_wallet_service->Reset();
}

void HnsDefaultExtensionsHandler::ResetTransactionInfo(
    const base::Value::List& args) {
  auto* tx_service =
      hns_wallet::TxServiceFactory::GetServiceForContext(profile_);
  if (tx_service)
    tx_service->Reset();
}

void HnsDefaultExtensionsHandler::SetWebTorrentEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  bool enabled = args[0].GetBool();

  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  extensions::ComponentLoader* loader = service->component_loader();

  if (enabled) {
    if (!loader->Exists(hns_webtorrent_extension_id)) {
      base::FilePath hns_webtorrent_path(FILE_PATH_LITERAL(""));
      hns_webtorrent_path =
          hns_webtorrent_path.Append(FILE_PATH_LITERAL("hns_webtorrent"));
      loader->Add(IDR_HNS_WEBTORRENT, hns_webtorrent_path);
    }
    service->EnableExtension(hns_webtorrent_extension_id);
  } else {
    service->DisableExtension(
        hns_webtorrent_extension_id,
        extensions::disable_reason::DisableReason::DISABLE_BLOCKED_BY_POLICY);
  }
}

void HnsDefaultExtensionsHandler::SetHangoutsEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  bool enabled = args[0].GetBool();

  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();

  if (enabled) {
    extensions::ComponentLoader* loader = service->component_loader();
    if (!loader->Exists(hangouts_extension_id)) {
      static_cast<extensions::HnsComponentLoader*>(loader)
          ->ForceAddHangoutServicesExtension();
    }
    service->EnableExtension(hangouts_extension_id);
  } else {
    service->DisableExtension(
        hangouts_extension_id,
        extensions::disable_reason::DisableReason::DISABLE_BLOCKED_BY_POLICY);
  }
}

bool HnsDefaultExtensionsHandler::IsExtensionInstalled(
    const std::string& extension_id) const {
  extensions::ExtensionRegistry* registry = extensions::ExtensionRegistry::Get(
      static_cast<content::BrowserContext*>(profile_));
  return registry && registry->GetInstalledExtension(extension_id);
}

void HnsDefaultExtensionsHandler::OnInstallResult(
    const std::string& pref_name,
    bool success,
    const std::string& error,
    extensions::webstore_install::Result result) {
  if (result != extensions::webstore_install::Result::SUCCESS &&
      result != extensions::webstore_install::Result::LAUNCH_IN_PROGRESS) {
    profile_->GetPrefs()->SetBoolean(pref_name, false);
  }
}

void HnsDefaultExtensionsHandler::OnRestartNeededChanged() {
  if (IsJavascriptAllowed()) {
    FireWebUIListener("hns-needs-restart-changed",
                      base::Value(IsRestartNeeded()));
  }
}

void HnsDefaultExtensionsHandler::SetWidevineEnabled(
    const base::Value::List& args) {
#if BUILDFLAG(ENABLE_WIDEVINE)
  CHECK_EQ(args.size(), 1U);
  bool enabled = args[0].GetBool();
  enabled ? EnableWidevineCdmComponent() : DisableWidevineCdmComponent();
  AllowJavascript();
#endif
}

void HnsDefaultExtensionsHandler::IsWidevineEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(args[0],
#if BUILDFLAG(ENABLE_WIDEVINE)
                            base::Value(IsWidevineOptedIn()));
#else
                            base::Value(false));
#endif
}

void HnsDefaultExtensionsHandler::OnWalletTypeChanged() {
  if (hns_wallet::GetDefaultEthereumWallet(profile_->GetPrefs()) ==
      hns_wallet::mojom::DefaultWallet::CryptoWallets)
    return;
  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  service->DisableExtension(
      ethereum_remote_client_extension_id,
      extensions::disable_reason::DisableReason::DISABLE_USER_ACTION);
}

void HnsDefaultExtensionsHandler::OnWidevineEnabledChanged() {
  if (IsJavascriptAllowed()) {
    FireWebUIListener("widevine-enabled-changed",
#if BUILDFLAG(ENABLE_WIDEVINE)
                      base::Value(IsWidevineOptedIn()));
#else
                      base::Value(false));
#endif
    OnRestartNeededChanged();
  }
}

void HnsDefaultExtensionsHandler::ExportIPNSKey(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  std::string key_name = args[0].GetString();
  DCHECK(!key_name.empty());
  auto* web_contents = web_ui()->GetWebContents();
  select_file_dialog_ = ui::SelectFileDialog::Create(
      this, std::make_unique<ChromeSelectFilePolicy>(web_contents));
  if (!select_file_dialog_) {
    VLOG(1) << "Export already in progress";
    return;
  }
  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  const base::FilePath directory = profile->last_selected_directory();
  gfx::NativeWindow parent_window = web_contents->GetTopLevelNativeWindow();
  ui::SelectFileDialog::FileTypeInfo file_types;
  file_types.allowed_paths = ui::SelectFileDialog::FileTypeInfo::NATIVE_PATH;
  dialog_key_ = key_name;
  auto suggested_directory = directory.AppendASCII(key_name);
  dialog_type_ = ui::SelectFileDialog::SELECT_SAVEAS_FILE;
  select_file_dialog_->SelectFile(
      ui::SelectFileDialog::SELECT_SAVEAS_FILE, base::UTF8ToUTF16(key_name),
      suggested_directory, &file_types, 0, FILE_PATH_LITERAL("key"),
      parent_window, nullptr);
}

void HnsDefaultExtensionsHandler::SetIPFSCompanionEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  bool enabled = args[0].GetBool();

  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  if (enabled) {
    if (!IsExtensionInstalled(ipfs_companion_extension_id)) {
      // Using FindLastActiveWithProfile() here will be fine. Of course, it can
      // return NULL but only return NULL when there was no activated window
      // with |profile_| so far. But, it's impossible at here because user can't
      // request ipfs install request w/o activating browser.
      scoped_refptr<extensions::WebstoreInstallWithPrompt> installer =
          new extensions::WebstoreInstallWithPrompt(
              ipfs_companion_extension_id, profile_,
              chrome::FindLastActiveWithProfile(profile_)
                  ->window()
                  ->GetNativeWindow(),
              base::BindOnce(&HnsDefaultExtensionsHandler::OnInstallResult,
                             weak_ptr_factory_.GetWeakPtr(),
                             kIPFSCompanionEnabled));
      installer->BeginInstall();
    }
    service->EnableExtension(ipfs_companion_extension_id);
  } else {
    service->DisableExtension(
        ipfs_companion_extension_id,
        extensions::disable_reason::DisableReason::DISABLE_USER_ACTION);
  }
}

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
void HnsDefaultExtensionsHandler::SetHnsWalletEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  bool enabled = args[0].GetBool();

  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  if (enabled) {
    service->EnableExtension(ethereum_remote_client_extension_id);
  } else {
    service->DisableExtension(
        ethereum_remote_client_extension_id,
        extensions::disable_reason::DisableReason::DISABLE_USER_ACTION);
  }
}
#endif

void HnsDefaultExtensionsHandler::GetDecentralizedDnsResolveMethodList(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();

  ResolveJavascriptCallback(args[0], ::GetResolveMethodList());
}

void HnsDefaultExtensionsHandler::GetEnsOffchainResolveMethodList(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();

  ResolveJavascriptCallback(args[0],
                            base::Value(::GetEnsOffchainResolveMethodList()));
}

#if BUILDFLAG(ENABLE_IPFS)
void HnsDefaultExtensionsHandler::LaunchIPFSService(
    const base::Value::List& args) {
  ipfs::IpfsService* service =
      ipfs::IpfsServiceFactory::GetForContext(profile_);
  if (!service) {
    return;
  }
  if (!service->IsDaemonLaunched())
    service->LaunchDaemon(base::NullCallback());
}

void HnsDefaultExtensionsHandler::SetIPFSStorageMax(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(args[0].is_int());
  CHECK(profile_);
  int storage_max_gb = args[0].GetInt();
  PrefService* prefs = Profile::FromWebUI(web_ui())->GetPrefs();
  prefs->SetInteger(kIpfsStorageMax, storage_max_gb);
  ipfs::IpfsService* service =
      ipfs::IpfsServiceFactory::GetForContext(profile_);
  if (!service) {
    return;
  }
  if (service->IsDaemonLaunched()) {
    service->RestartDaemon();
  }
}

void HnsDefaultExtensionsHandler::FileSelected(const base::FilePath& path,
                                                 int index,
                                                 void* params) {
  ipfs::IpfsService* service =
      ipfs::IpfsServiceFactory::GetForContext(profile_);
  if (!service)
    return;
  if (dialog_type_ == ui::SelectFileDialog::SELECT_OPEN_FILE) {
    service->GetIpnsKeysManager()->ImportKey(
        path, dialog_key_,
        base::BindOnce(&HnsDefaultExtensionsHandler::OnKeyImported,
                       weak_ptr_factory_.GetWeakPtr()));
  } else if (dialog_type_ == ui::SelectFileDialog::SELECT_SAVEAS_FILE) {
    service->ExportKey(
        dialog_key_, path,
        base::BindOnce(&HnsDefaultExtensionsHandler::OnKeyExported,
                       weak_ptr_factory_.GetWeakPtr(), dialog_key_));
  }
  dialog_type_ = ui::SelectFileDialog::SELECT_NONE;
  select_file_dialog_.reset();
  dialog_key_.clear();
}

void HnsDefaultExtensionsHandler::OnKeyExported(const std::string& key,
                                                  bool success) {
  if (!IsJavascriptAllowed())
    return;
  FireWebUIListener("hns-ipfs-key-exported", base::Value(key),
                    base::Value(success));
}

void HnsDefaultExtensionsHandler::OnKeyImported(const std::string& key,
                                                  const std::string& value,
                                                  bool success) {
  if (!IsJavascriptAllowed())
    return;
  FireWebUIListener("hns-ipfs-key-imported", base::Value(key),
                    base::Value(value), base::Value(success));
}

void HnsDefaultExtensionsHandler::FileSelectionCanceled(void* params) {
  select_file_dialog_.reset();
  dialog_key_.clear();
}

void HnsDefaultExtensionsHandler::ImportIpnsKey(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  std::string key_name = args[0].GetString();
  auto* web_contents = web_ui()->GetWebContents();
  select_file_dialog_ = ui::SelectFileDialog::Create(
      this, std::make_unique<ChromeSelectFilePolicy>(web_contents));
  if (!select_file_dialog_) {
    VLOG(1) << "Export already in progress";
    return;
  }
  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  const base::FilePath directory = profile->last_selected_directory();
  gfx::NativeWindow parent_window = web_contents->GetTopLevelNativeWindow();
  ui::SelectFileDialog::FileTypeInfo file_types;
  file_types.allowed_paths = ui::SelectFileDialog::FileTypeInfo::NATIVE_PATH;
  dialog_key_ = key_name;
  dialog_type_ = ui::SelectFileDialog::SELECT_OPEN_FILE;
  select_file_dialog_->SelectFile(
      ui::SelectFileDialog::SELECT_OPEN_FILE, std::u16string(), directory,
      &file_types, 0, FILE_PATH_LITERAL("key"), parent_window, nullptr);
}

void HnsDefaultExtensionsHandler::CheckIpfsNodeStatus(
    const base::Value::List& args) {
  NotifyNodeStatus();
}

void HnsDefaultExtensionsHandler::NotifyNodeStatus() {
  ipfs::IpfsService* service =
      ipfs::IpfsServiceFactory::GetForContext(profile_);
  if (!IsJavascriptAllowed())
    return;
  bool launched = service && service->IsDaemonLaunched();
  FireWebUIListener("hns-ipfs-node-status-changed", base::Value(launched));
}

void HnsDefaultExtensionsHandler::OnIpfsLaunched(bool result, int64_t pid) {
  if (!IsJavascriptAllowed())
    return;
  NotifyNodeStatus();
}

void HnsDefaultExtensionsHandler::OnIpfsShutdown() {
  if (!IsJavascriptAllowed())
    return;
  NotifyNodeStatus();
}
void HnsDefaultExtensionsHandler::OnIpnsKeysLoaded(bool success) {
  if (!IsJavascriptAllowed())
    return;
  FireWebUIListener("hns-ipfs-keys-loaded", base::Value(success));
}
#endif
