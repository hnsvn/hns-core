/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_TRIGGER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_TRIGGER_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
namespace credential {

struct CredentialsTrigger {
  CredentialsTrigger();
  CredentialsTrigger(const CredentialsTrigger& info);
  ~CredentialsTrigger();

  std::string id;
  mojom::CredsBatchType type;
  int size;
  std::vector<std::string> data;
};

}  // namespace credential
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_TRIGGER_H_
