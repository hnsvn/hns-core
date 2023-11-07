/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ATTESTATION_ATTESTATION_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ATTESTATION_ATTESTATION_H_

#include <map>
#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace attestation {

using StartCallback =
    base::OnceCallback<void(mojom::Result, const std::string&)>;

using ConfirmCallback = base::OnceCallback<void(mojom::Result)>;

class Attestation {
 public:
  explicit Attestation(RewardsEngineImpl& engine);
  virtual ~Attestation();

  virtual void Start(const std::string& payload, StartCallback callback) = 0;

  virtual void Confirm(const std::string& solution,
                       ConfirmCallback callback) = 0;

 protected:
  const raw_ref<RewardsEngineImpl> engine_;
};

}  // namespace attestation
}  // namespace hns_rewards::internal
#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ATTESTATION_ATTESTATION_H_
