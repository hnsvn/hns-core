/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_MULTI_ARMED_BANDITS_RESOURCE_EPSILON_GREEDY_BANDIT_RESOURCE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_MULTI_ARMED_BANDITS_RESOURCE_EPSILON_GREEDY_BANDIT_RESOURCE_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_observer.h"
#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"
#include "hns/components/hns_ads/core/public/client/ads_client_notifier_observer.h"

namespace hns_ads {

class Catalog;
struct CatalogInfo;

class EpsilonGreedyBanditResource final : public AdsClientNotifierObserver,
                                          public CatalogObserver {
 public:
  explicit EpsilonGreedyBanditResource(Catalog& catalog);

  EpsilonGreedyBanditResource(const EpsilonGreedyBanditResource&) = delete;
  EpsilonGreedyBanditResource& operator=(const EpsilonGreedyBanditResource&) =
      delete;

  EpsilonGreedyBanditResource(EpsilonGreedyBanditResource&&) noexcept = delete;
  EpsilonGreedyBanditResource& operator=(
      EpsilonGreedyBanditResource&&) noexcept = delete;

  ~EpsilonGreedyBanditResource() override;

  bool IsInitialized() const { return is_initialized_; }

  static SegmentList Get();

 private:
  void LoadFromCatalog(const CatalogInfo& catalog);

  void MaybeReset();
  void Reset();

  // AdsClientNotifierObserver:
  void OnNotifyPrefDidChange(const std::string& path) override;

  // CatalogObserver:
  void OnDidUpdateCatalog(const CatalogInfo& catalog) override;

  bool is_initialized_ = false;

  const raw_ref<Catalog> catalog_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_MULTI_ARMED_BANDITS_RESOURCE_EPSILON_GREEDY_BANDIT_RESOURCE_H_
