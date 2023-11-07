/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_PUBLISHER_STATE_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_PUBLISHER_STATE_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/legacy/publisher_settings_properties.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;
}

namespace hns_rewards::internal {
namespace publisher {

class LegacyPublisherState {
 public:
  explicit LegacyPublisherState(RewardsEngineImpl& engine);

  ~LegacyPublisherState();

  uint64_t GetPublisherMinVisitTime() const;  // In milliseconds

  unsigned int GetPublisherMinVisits() const;

  bool GetPublisherAllowNonVerified() const;

  void Load(LegacyResultCallback callback);

  std::vector<std::string> GetAlreadyProcessedPublishers() const;

  void GetAllBalanceReports(std::vector<mojom::BalanceReportInfoPtr>* reports);

 private:
  void OnLoad(LegacyResultCallback callback,
              mojom::Result result,
              const std::string& data);

  const raw_ref<RewardsEngineImpl> engine_;
  PublisherSettingsProperties state_;
};

}  // namespace publisher
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_PUBLISHER_STATE_H_
