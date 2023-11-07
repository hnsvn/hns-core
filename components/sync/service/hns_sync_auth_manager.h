/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_SYNC_SERVICE_HNS_SYNC_AUTH_MANAGER_H_
#define HNS_COMPONENTS_SYNC_SERVICE_HNS_SYNC_AUTH_MANAGER_H_

#include <string>
#include <vector>

#include "base/time/time.h"
#include "components/sync/service/sync_auth_manager.h"

namespace syncer {

class HnsSyncAuthManager : public SyncAuthManager {
 public:
  HnsSyncAuthManager(signin::IdentityManager* identity_manager,
                       const AccountStateChangedCallback& account_state_changed,
                       const CredentialsChangedCallback& credentials_changed);
  HnsSyncAuthManager(const HnsSyncAuthManager&) = delete;
  HnsSyncAuthManager& operator=(const HnsSyncAuthManager&) = delete;
  ~HnsSyncAuthManager() override;

  void DeriveSigningKeys(const std::string& seed);
  void ResetKeys();

  void RequestAccessToken() override;

  // signin::IdentityManager::Observer implementation.
  void OnRefreshTokenUpdatedForAccount(
      const CoreAccountInfo& account_info) override {}
  void OnRefreshTokenRemovedForAccount(
      const CoreAccountId& account_id) override {}
  void OnRefreshTokensLoaded() override {}

 private:
  SyncAccountInfo DetermineAccountToUse() const override;
  std::string GenerateAccessToken(const std::string& timestamp);
  void OnNetworkTimeFetched(const base::Time& time);

  std::vector<uint8_t> public_key_;
  std::vector<uint8_t> private_key_;

  base::WeakPtrFactory<HnsSyncAuthManager> weak_ptr_factory_{this};
};

}  // namespace syncer

#endif  // HNS_COMPONENTS_SYNC_SERVICE_HNS_SYNC_AUTH_MANAGER_H_
