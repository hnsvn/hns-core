/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_REDEEM_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_REDEEM_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
namespace credential {

struct CredentialsRedeem {
  CredentialsRedeem();
  CredentialsRedeem(const CredentialsRedeem& info);
  ~CredentialsRedeem();

  std::string publisher_key;
  mojom::RewardsType type;
  mojom::ContributionProcessor processor;
  std::vector<mojom::UnblindedToken> token_list;
  std::string order_id;
  std::string contribution_id;
};

}  // namespace credential
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_REDEEM_H_
