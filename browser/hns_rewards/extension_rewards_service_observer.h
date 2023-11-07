/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_REWARDS_EXTENSION_REWARDS_SERVICE_OBSERVER_H_
#define HNS_BROWSER_HNS_REWARDS_EXTENSION_REWARDS_SERVICE_OBSERVER_H_

#include <memory>
#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "hns/components/hns_rewards/browser/rewards_service_observer.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"

class Profile;

namespace hns_rewards {

class RewardsService;

class ExtensionRewardsServiceObserver : public RewardsServiceObserver {
 public:
  explicit ExtensionRewardsServiceObserver(Profile* profile);

  ExtensionRewardsServiceObserver(const ExtensionRewardsServiceObserver&) =
      delete;

  ExtensionRewardsServiceObserver& operator=(
      const ExtensionRewardsServiceObserver&) = delete;

  ~ExtensionRewardsServiceObserver() override;

  // RewardsServiceObserver:
  void OnRewardsInitialized(RewardsService* rewards_service) override;

  void OnRewardsWalletCreated() override;

  void OnPublisherListNormalized(
      RewardsService* rewards_service,
      std::vector<hns_rewards::mojom::PublisherInfoPtr> list) override;

  void OnExcludedSitesChanged(RewardsService* rewards_service,
                              std::string publisher_key,
                              bool excluded) override;

  void OnRecurringTipSaved(RewardsService* rewards_service,
                           bool success) override;

  void OnRecurringTipRemoved(RewardsService* rewards_service,
                             bool success) override;

  void OnReconcileComplete(
      RewardsService* rewards_service,
      const hns_rewards::mojom::Result result,
      const std::string& contribution_id,
      const double amount,
      const hns_rewards::mojom::RewardsType type,
      const hns_rewards::mojom::ContributionProcessor processor) override;

  void OnExternalWalletConnected() override;

  void OnExternalWalletLoggedOut() override;

  void OnExternalWalletDisconnected() override;

  void OnUnblindedTokensReady(
      hns_rewards::RewardsService* rewards_service) override;

  void OnFetchPromotions(
      RewardsService* rewards_service,
      const hns_rewards::mojom::Result result,
      const std::vector<hns_rewards::mojom::PromotionPtr>& list) override;

  void OnPromotionFinished(
      RewardsService* rewards_service,
      const hns_rewards::mojom::Result result,
      hns_rewards::mojom::PromotionPtr promotion) override;

  void OnCompleteReset(const bool success) override;

  void OnPanelPublisherInfo(RewardsService* rewards_service,
                            const hns_rewards::mojom::Result result,
                            const hns_rewards::mojom::PublisherInfo* info,
                            uint64_t windowId) override;

 private:
  raw_ptr<Profile> profile_ = nullptr;
};

}  // namespace hns_rewards

#endif  // HNS_BROWSER_HNS_REWARDS_EXTENSION_REWARDS_SERVICE_OBSERVER_H_
