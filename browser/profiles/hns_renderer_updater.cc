/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/profiles/hns_renderer_updater.h"

#include <utility>

#include "base/functional/bind.h"
#include "hns/browser/hns_wallet/hns_wallet_context_utils.h"
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_constants.h"
#include "hns/common/hns_renderer_configuration.mojom.h"
#include "hns/components/hns_wallet/browser/keyring_service.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wallet/common/common_utils.h"
#include "hns/components/de_amp/browser/de_amp_util.h"
#include "hns/components/de_amp/common/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_process_host.h"
#include "extensions/buildflags/buildflags.h"
#include "ipc/ipc_channel_proxy.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "extensions/browser/extension_registry.h"
#endif

HnsRendererUpdater::HnsRendererUpdater(
    Profile* profile,
    hns_wallet::KeyringService* keyring_service)
    : profile_(profile), keyring_service_(keyring_service) {
  PrefService* pref_service = profile->GetPrefs();
  hns_wallet_ethereum_provider_.Init(kDefaultEthereumWallet, pref_service);
  hns_wallet_solana_provider_.Init(kDefaultSolanaWallet, pref_service);
  de_amp_enabled_.Init(de_amp::kDeAmpPrefEnabled, pref_service);

  CheckActiveWallet();

  pref_change_registrar_.Init(pref_service);
  pref_change_registrar_.Add(
      kDefaultEthereumWallet,
      base::BindRepeating(&HnsRendererUpdater::UpdateAllRenderers,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kDefaultSolanaWallet,
      base::BindRepeating(&HnsRendererUpdater::UpdateAllRenderers,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      de_amp::kDeAmpPrefEnabled,
      base::BindRepeating(&HnsRendererUpdater::UpdateAllRenderers,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      kHnsWalletKeyrings,
      base::BindRepeating(
          &HnsRendererUpdater::CheckActiveWalletAndMaybeUpdateRenderers,
          base::Unretained(this)));
}

HnsRendererUpdater::~HnsRendererUpdater() = default;

void HnsRendererUpdater::InitializeRenderer(
    content::RenderProcessHost* render_process_host) {
  auto renderer_configuration = GetRendererConfiguration(render_process_host);
  Profile* profile =
      Profile::FromBrowserContext(render_process_host->GetBrowserContext());
  is_wallet_allowed_for_context_ = hns_wallet::IsAllowedForContext(profile);
  renderer_configuration->SetInitialConfiguration(profile->IsTor());
  UpdateRenderer(&renderer_configuration);
}

std::vector<mojo::AssociatedRemote<hns::mojom::HnsRendererConfiguration>>
HnsRendererUpdater::GetRendererConfigurations() {
  std::vector<mojo::AssociatedRemote<hns::mojom::HnsRendererConfiguration>>
      rv;
  for (content::RenderProcessHost::iterator it(
           content::RenderProcessHost::AllHostsIterator());
       !it.IsAtEnd(); it.Advance()) {
    Profile* renderer_profile =
        static_cast<Profile*>(it.GetCurrentValue()->GetBrowserContext());
    if (renderer_profile == profile_ ||
        renderer_profile->GetOriginalProfile() == profile_) {
      auto renderer_configuration =
          GetRendererConfiguration(it.GetCurrentValue());
      if (renderer_configuration) {
        rv.push_back(std::move(renderer_configuration));
      }
    }
  }
  return rv;
}

mojo::AssociatedRemote<hns::mojom::HnsRendererConfiguration>
HnsRendererUpdater::GetRendererConfiguration(
    content::RenderProcessHost* render_process_host) {
  IPC::ChannelProxy* channel = render_process_host->GetChannel();
  if (!channel) {
    return mojo::AssociatedRemote<hns::mojom::HnsRendererConfiguration>();
  }

  mojo::AssociatedRemote<hns::mojom::HnsRendererConfiguration>
      renderer_configuration;
  channel->GetRemoteAssociatedInterface(&renderer_configuration);

  return renderer_configuration;
}

void HnsRendererUpdater::CheckActiveWalletAndMaybeUpdateRenderers() {
  if (CheckActiveWallet()) {
    UpdateAllRenderers();
  }
}

bool HnsRendererUpdater::CheckActiveWallet() {
  if (!keyring_service_) {
    return false;
  }
  bool is_wallet_created = keyring_service_->IsKeyringCreated(
      hns_wallet::mojom::kDefaultKeyringId);
  bool changed = is_wallet_created != is_wallet_created_;
  is_wallet_created_ = is_wallet_created;
  return changed;
}

void HnsRendererUpdater::UpdateAllRenderers() {
  auto renderer_configurations = GetRendererConfigurations();
  for (auto& renderer_configuration : renderer_configurations) {
    UpdateRenderer(&renderer_configuration);
  }
}

void HnsRendererUpdater::UpdateRenderer(
    mojo::AssociatedRemote<hns::mojom::HnsRendererConfiguration>*
        renderer_configuration) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(profile_);
  bool has_installed_metamask =
      registry &&
      registry->enabled_extensions().Contains(metamask_extension_id);
#else
  bool has_installed_metamask = false;
#endif

  bool should_ignore_hns_wallet_for_eth =
      !is_wallet_created_ || has_installed_metamask;
  bool should_ignore_hns_wallet_for_sol = !is_wallet_created_;

  auto default_ethereum_wallet =
      static_cast<hns_wallet::mojom::DefaultWallet>(
          hns_wallet_ethereum_provider_.GetValue());
  bool install_window_hns_ethereum_provider =
      is_wallet_allowed_for_context_ && hns_wallet::IsDappsSupportEnabled() &&
      default_ethereum_wallet != hns_wallet::mojom::DefaultWallet::None;
  bool install_window_ethereum_provider =
      ((default_ethereum_wallet ==
            hns_wallet::mojom::DefaultWallet::HnsWalletPreferExtension &&
        !should_ignore_hns_wallet_for_eth) ||
       default_ethereum_wallet ==
           hns_wallet::mojom::DefaultWallet::HnsWallet) &&
      is_wallet_allowed_for_context_ && hns_wallet::IsDappsSupportEnabled();
  bool allow_overwrite_window_ethereum_provider =
      default_ethereum_wallet ==
      hns_wallet::mojom::DefaultWallet::HnsWalletPreferExtension;

  auto default_solana_wallet = static_cast<hns_wallet::mojom::DefaultWallet>(
      hns_wallet_solana_provider_.GetValue());
  bool hns_use_native_solana_wallet =
      ((default_solana_wallet ==
            hns_wallet::mojom::DefaultWallet::HnsWalletPreferExtension &&
        !should_ignore_hns_wallet_for_sol) ||
       default_solana_wallet ==
           hns_wallet::mojom::DefaultWallet::HnsWallet) &&
      is_wallet_allowed_for_context_ && hns_wallet::IsDappsSupportEnabled();
  bool allow_overwrite_window_solana_provider =
      default_solana_wallet ==
      hns_wallet::mojom::DefaultWallet::HnsWalletPreferExtension;

  PrefService* pref_service = profile_->GetPrefs();
  bool de_amp_enabled = de_amp::IsDeAmpEnabled(pref_service);

  (*renderer_configuration)
      ->SetConfiguration(hns::mojom::DynamicParams::New(
          install_window_hns_ethereum_provider,
          install_window_ethereum_provider,
          allow_overwrite_window_ethereum_provider,
          hns_use_native_solana_wallet,
          allow_overwrite_window_solana_provider, de_amp_enabled));
}
