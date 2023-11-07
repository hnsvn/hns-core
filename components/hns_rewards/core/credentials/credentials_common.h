/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_COMMON_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_COMMON_H_

#include <stdint.h>

#include <map>
#include <string>
#include <vector>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/credentials/credentials.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace credential {

class CredentialsCommon {
 public:
  explicit CredentialsCommon(RewardsEngineImpl& engine);
  ~CredentialsCommon();

  void GetBlindedCreds(const CredentialsTrigger& trigger,
                       ResultCallback callback);

  void SaveUnblindedCreds(
      uint64_t expires_at,
      double token_value,
      const mojom::CredsBatch& creds,
      const std::vector<std::string>& unblinded_encoded_creds,
      const CredentialsTrigger& trigger,
      ResultCallback callback);

 private:
  void BlindedCredsSaved(ResultCallback callback, mojom::Result result);

  void OnSaveUnblindedCreds(ResultCallback callback,
                            const CredentialsTrigger& trigger,
                            mojom::Result result);

  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace credential
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_COMMON_H_
