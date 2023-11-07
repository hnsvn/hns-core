/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/hns_component_loader.h"

#include <string>

#include "base/command_line.h"
#include "base/feature_list.h"
#include "base/functional/bind.h"
#include "hns/browser/hns_rewards/rewards_util.h"
#include "hns/components/hns_component_updater/browser/hns_component_installer.h"
#include "hns/components/hns_component_updater/browser/hns_on_demand_updater.h"
#include "hns/components/hns_extension/grit/hns_extension.h"
#include "hns/components/hns_webtorrent/grit/hns_webtorrent_resources.h"
#include "hns/components/constants/hns_switches.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "components/grit/hns_components_resources.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_service.h"
#include "extensions/browser/extension_prefs.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/constants.h"
#include "extensions/common/mojom/manifest.mojom.h"

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_constants.h"
#include "hns/browser/ethereum_remote_client/pref_names.h"
#include "hns/browser/extensions/ethereum_remote_client_util.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#endif

using extensions::mojom::ManifestLocation;

namespace extensions {

HnsComponentLoader::HnsComponentLoader(ExtensionSystem* extension_system,
                                           Profile* profile)
    : ComponentLoader(extension_system, profile),
      profile_(profile),
      profile_prefs_(profile->GetPrefs()) {}

HnsComponentLoader::~HnsComponentLoader() = default;

void HnsComponentLoader::OnComponentRegistered(std::string extension_id) {
  hns_component_updater::HnsOnDemandUpdater::GetInstance()->OnDemandUpdate(
      extension_id);
}

void HnsComponentLoader::OnComponentReady(std::string extension_id,
                                            bool allow_file_access,
                                            const base::FilePath& install_dir,
                                            const std::string& manifest) {
  Add(manifest, install_dir);
  if (allow_file_access) {
    ExtensionPrefs::Get(profile_)->SetAllowFileAccess(extension_id, true);
  }
#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
  if (extension_id == ethereum_remote_client_extension_id) {
    ReinstallAsNonComponent(ethereum_remote_client_extension_id);
  }
#endif
}

void HnsComponentLoader::ReinstallAsNonComponent(
    const std::string extension_id) {
  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(profile_);
  const Extension* extension = registry->GetInstalledExtension(extension_id);
  DCHECK(extension);
  if (extension->location() == ManifestLocation::kComponent) {
    service->RemoveComponentExtension(extension_id);
    std::string error;
    scoped_refptr<Extension> normal_extension = Extension::Create(
        extension->path(), ManifestLocation::kExternalPref,
        *extension->manifest()->value(), extension->creation_flags(), &error);
    service->AddExtension(normal_extension.get());
  }
}

void HnsComponentLoader::AddExtension(const std::string& extension_id,
                                        const std::string& name,
                                        const std::string& public_key) {
  hns::RegisterComponent(
      g_browser_process->component_updater(), name, public_key,
      base::BindOnce(&HnsComponentLoader::OnComponentRegistered,
                     base::Unretained(this), extension_id),
      base::BindRepeating(&HnsComponentLoader::OnComponentReady,
                          base::Unretained(this), extension_id, true));
}

void HnsComponentLoader::AddHangoutServicesExtension() {
  if (!profile_prefs_->FindPreference(kHangoutsEnabled) ||
      profile_prefs_->GetBoolean(kHangoutsEnabled)) {
    ForceAddHangoutServicesExtension();
  }
}

void HnsComponentLoader::ForceAddHangoutServicesExtension() {
  ComponentLoader::AddHangoutServicesExtension();
}

void HnsComponentLoader::AddDefaultComponentExtensions(
    bool skip_session_components) {
  ComponentLoader::AddDefaultComponentExtensions(skip_session_components);

  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kDisableHnsExtension)) {
    base::FilePath hns_extension_path(FILE_PATH_LITERAL(""));
    hns_extension_path =
        hns_extension_path.Append(FILE_PATH_LITERAL("hns_extension"));
    Add(IDR_HNS_EXTENSION, hns_extension_path);
  }
}

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
void HnsComponentLoader::AddEthereumRemoteClientExtension() {
  AddExtension(ethereum_remote_client_extension_id,
               ethereum_remote_client_extension_name,
               ethereum_remote_client_extension_public_key);
}

void HnsComponentLoader::AddEthereumRemoteClientExtensionOnStartup() {
  // Only load Crypto Wallets if it is set as the default wallet
  auto default_wallet = hns_wallet::GetDefaultEthereumWallet(profile_prefs_);
  const bool is_opted_into_cw =
      profile_prefs_->GetBoolean(kERCOptedIntoCryptoWallets);
  if (HasInfuraProjectID() && is_opted_into_cw &&
      default_wallet == hns_wallet::mojom::DefaultWallet::CryptoWallets) {
    AddEthereumRemoteClientExtension();
  }
}

void HnsComponentLoader::UnloadEthereumRemoteClientExtension() {
  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  DCHECK(service);
  service->UnloadExtension(ethereum_remote_client_extension_id,
                           extensions::UnloadedExtensionReason::DISABLE);
}
#endif

void HnsComponentLoader::AddWebTorrentExtension() {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kDisableWebTorrentExtension) &&
      (!profile_prefs_->FindPreference(kWebTorrentEnabled) ||
       profile_prefs_->GetBoolean(kWebTorrentEnabled))) {
    base::FilePath hns_webtorrent_path(FILE_PATH_LITERAL(""));
    hns_webtorrent_path =
        hns_webtorrent_path.Append(FILE_PATH_LITERAL("hns_webtorrent"));
    Add(IDR_HNS_WEBTORRENT, hns_webtorrent_path);
  }
}

}  // namespace extensions
