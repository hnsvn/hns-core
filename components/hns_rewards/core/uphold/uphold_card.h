/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_UPHOLD_UPHOLD_CARD_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_UPHOLD_UPHOLD_CARD_H_

#include <string>

#include "base/functional/callback_forward.h"
#include "hns/components/hns_rewards/common/mojom/rewards.mojom.h"
#include "hns/components/hns_rewards/core/endpoint/uphold/uphold_server.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace uphold {

const char kCardName[] = "Hns Browser";

using CreateCardCallback =
    base::OnceCallback<void(mojom::Result, std::string&& id)>;

class UpholdCard {
 public:
  explicit UpholdCard(RewardsEngineImpl& engine);

  ~UpholdCard();

  void CreateBATCardIfNecessary(const std::string& access_token,
                                CreateCardCallback) const;

 private:
  void OnGetBATCardId(CreateCardCallback,
                      const std::string& access_token,
                      mojom::Result,
                      std::string&& id) const;

  void OnCreateBATCard(CreateCardCallback,
                       const std::string& access_token,
                       mojom::Result,
                       std::string&& id) const;

  void OnUpdateBATCardSettings(CreateCardCallback,
                               std::string&& id,
                               mojom::Result) const;

  endpoint::UpholdServer uphold_server_;
};

}  // namespace uphold
}  // namespace hns_rewards::internal
#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_UPHOLD_UPHOLD_CARD_H_
