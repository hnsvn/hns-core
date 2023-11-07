/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_MULTI_ARMED_BANDITS_EPSILON_GREEDY_BANDIT_PROCESSOR_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_MULTI_ARMED_BANDITS_EPSILON_GREEDY_BANDIT_PROCESSOR_H_

#include <string>

#include "hns/components/hns_ads/core/public/client/ads_client_notifier_observer.h"

namespace hns_ads {

struct EpsilonGreedyBanditFeedbackInfo;

class EpsilonGreedyBanditProcessor final : public AdsClientNotifierObserver {
 public:
  EpsilonGreedyBanditProcessor();

  EpsilonGreedyBanditProcessor(const EpsilonGreedyBanditProcessor&) = delete;
  EpsilonGreedyBanditProcessor& operator=(const EpsilonGreedyBanditProcessor&) =
      delete;

  EpsilonGreedyBanditProcessor(EpsilonGreedyBanditProcessor&&) noexcept =
      delete;
  EpsilonGreedyBanditProcessor& operator=(
      EpsilonGreedyBanditProcessor&&) noexcept = delete;

  ~EpsilonGreedyBanditProcessor() override;

  void Process(const EpsilonGreedyBanditFeedbackInfo& feedback) const;

  bool IsInitialized() const { return is_initialized_; }

 private:
  void MaybeInitializeArms();
  void MaybeInitializeOrResetArms();
  void InitializeArms();
  void ResetArms();

  // AdsClientNotifierObserver:
  void OnNotifyDidInitializeAds() override;
  void OnNotifyPrefDidChange(const std::string& path) override;

  bool is_initialized_ = false;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_MULTI_ARMED_BANDITS_EPSILON_GREEDY_BANDIT_PROCESSOR_H_
