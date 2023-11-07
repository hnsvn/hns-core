/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/hns_extension_management.h"

#include <memory>

#include "hns/browser/hns_browser_process.h"
#include "hns/browser/ethereum_remote_client/buildflags/buildflags.h"
#include "hns/browser/extensions/hns_extension_provider.h"
#include "hns/browser/tor/tor_profile_service_factory.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/extensions/extension_management_internal.h"
#include "chrome/browser/extensions/external_policy_loader.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/common/constants.h"
#include "extensions/common/extension.h"
#include "extensions/common/extension_urls.h"

#if BUILDFLAG(ENABLE_TOR)
#include "hns/browser/tor/tor_profile_manager.h"
#include "hns/components/tor/hns_tor_client_updater.h"
#include "hns/components/tor/hns_tor_pluggable_transport_updater.h"
#include "hns/components/tor/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/components/ipfs/hns_ipfs_client_updater.h"
#include "hns/components/ipfs/ipfs_utils.h"
#include "components/user_prefs/user_prefs.h"
#endif

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_constants.h"
#endif

namespace extensions {

HnsExtensionManagement::HnsExtensionManagement(Profile* profile)
    : ExtensionManagement(profile) {
  extension_registry_observer_.Observe(
      ExtensionRegistry::Get(static_cast<content::BrowserContext*>(profile)));
  providers_.push_back(std::make_unique<HnsExtensionProvider>());
  local_state_pref_change_registrar_.Init(g_browser_process->local_state());
#if BUILDFLAG(ENABLE_TOR)
  local_state_pref_change_registrar_.Add(
      tor::prefs::kTorDisabled,
      base::BindRepeating(&HnsExtensionManagement::OnTorDisabledChanged,
                          base::Unretained(this)));
  local_state_pref_change_registrar_.Add(
      tor::prefs::kBridgesConfig,
      base::BindRepeating(
          &HnsExtensionManagement::OnTorPluggableTransportChanged,
          base::Unretained(this)));
#endif
  // Make IsInstallationExplicitlyAllowed to be true
#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
  AccessById(ethereum_remote_client_extension_id)->installation_mode =
      INSTALLATION_RECOMMENDED;
#endif
  Cleanup(profile);
}

HnsExtensionManagement::~HnsExtensionManagement() {
  local_state_pref_change_registrar_.RemoveAll();
}

void HnsExtensionManagement::OnExtensionLoaded(
    content::BrowserContext* browser_context,
    const Extension* extension) {
  if (extension->id() == ipfs_companion_extension_id)
    pref_service_->SetBoolean(kIPFSCompanionEnabled, true);
}

void HnsExtensionManagement::OnExtensionUnloaded(
    content::BrowserContext* browser_context,
    const Extension* extension,
    UnloadedExtensionReason reason) {
  if (extension->id() == ipfs_companion_extension_id)
    pref_service_->SetBoolean(kIPFSCompanionEnabled, false);
}

void HnsExtensionManagement::OnTorDisabledChanged() {
#if BUILDFLAG(ENABLE_TOR)
  if (TorProfileServiceFactory::IsTorDisabled(profile_)) {
    TorProfileManager::GetInstance().CloseAllTorWindows();
    if (g_hns_browser_process->tor_client_updater()) {
      g_hns_browser_process->tor_client_updater()->Cleanup();
    }
    if (g_hns_browser_process->tor_pluggable_transport_updater()) {
      g_hns_browser_process->tor_pluggable_transport_updater()->Cleanup();
    }
  }
#endif
}

void HnsExtensionManagement::OnTorPluggableTransportChanged() {
#if BUILDFLAG(ENABLE_TOR)
  if (TorProfileServiceFactory::GetTorBridgesConfig().use_bridges ==
      tor::BridgesConfig::Usage::kNotUsed) {
    if (g_hns_browser_process->tor_pluggable_transport_updater()) {
      g_hns_browser_process->tor_pluggable_transport_updater()->Cleanup();
    }
  }
#endif
}

void HnsExtensionManagement::Cleanup(content::BrowserContext* context) {
  // BrowserPolicyConnector enforce policy earlier than this constructor so we
  // have to manully cleanup tor executable when tor is disabled by gpo
  OnTorDisabledChanged();
  OnTorPluggableTransportChanged();

#if BUILDFLAG(ENABLE_IPFS)
  // Remove ipfs executable if it is disabled by GPO.
  if (ipfs::IsIpfsDisabledByPolicy(user_prefs::UserPrefs::Get(context)))
    g_hns_browser_process->ipfs_client_updater()->Cleanup();
#endif
}

}  // namespace extensions
