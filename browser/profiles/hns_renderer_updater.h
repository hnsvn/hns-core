/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_PROFILES_HNS_RENDERER_UPDATER_H_
#define HNS_BROWSER_PROFILES_HNS_RENDERER_UPDATER_H_

#include <vector>

#include "base/memory/raw_ptr.h"
#include "hns/common/hns_renderer_configuration.mojom-forward.h"
#include "components/keyed_service/core/keyed_service.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_member.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "mojo/public/cpp/bindings/remote.h"

class Profile;

namespace hns_wallet {
class KeyringService;
}

namespace content {
class RenderProcessHost;
}

class HnsRendererUpdater : public KeyedService {
 public:
  HnsRendererUpdater(Profile* profile,
                       hns_wallet::KeyringService* keyring_service);
  HnsRendererUpdater(const HnsRendererUpdater&) = delete;
  HnsRendererUpdater& operator=(const HnsRendererUpdater&) = delete;
  ~HnsRendererUpdater() override;

  // Initialize a newly-started renderer process.
  void InitializeRenderer(content::RenderProcessHost* render_process_host);

 private:
  std::vector<mojo::AssociatedRemote<hns::mojom::HnsRendererConfiguration>>
  GetRendererConfigurations();

  mojo::AssociatedRemote<hns::mojom::HnsRendererConfiguration>
  GetRendererConfiguration(content::RenderProcessHost* render_process_host);

  // Update renderers if wallet keyring has been initialized
  void CheckActiveWalletAndMaybeUpdateRenderers();

  // Update active wallet bool, returns true if status has changed
  bool CheckActiveWallet();

  // Update all renderers due to a configuration change.
  void UpdateAllRenderers();

  // Update the given renderer due to a configuration change.
  void UpdateRenderer(
      mojo::AssociatedRemote<hns::mojom::HnsRendererConfiguration>*
          renderer_configuration);

  raw_ptr<Profile> profile_ = nullptr;
  raw_ptr<hns_wallet::KeyringService> keyring_service_ = nullptr;
  PrefChangeRegistrar pref_change_registrar_;

  // Prefs that we sync to the renderers.
  IntegerPrefMember hns_wallet_ethereum_provider_;
  IntegerPrefMember hns_wallet_solana_provider_;
  BooleanPrefMember de_amp_enabled_;
  bool is_wallet_allowed_for_context_ = false;
  bool is_wallet_created_ = false;
};

#endif  // HNS_BROWSER_PROFILES_HNS_RENDERER_UPDATER_H_
