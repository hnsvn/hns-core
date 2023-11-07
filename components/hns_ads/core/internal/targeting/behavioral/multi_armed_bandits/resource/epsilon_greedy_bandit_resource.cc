/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/behavioral/multi_armed_bandits/resource/epsilon_greedy_bandit_resource.h"

#include "hns/components/hns_ads/core/internal/catalog/catalog.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_info.h"
#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/segments/segment_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/multi_armed_bandits/resource/epsilon_greedy_bandit_resource_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "hns/components/hns_rewards/common/pref_names.h"

namespace hns_ads {

namespace {

bool DoesRequireResource() {
  return UserHasOptedInToHnsNewsAds() || UserHasOptedInToNotificationAds();
}

}  // namespace

EpsilonGreedyBanditResource::EpsilonGreedyBanditResource(Catalog& catalog)
    : catalog_(catalog) {
  AdsClientHelper::AddObserver(this);
  catalog_->AddObserver(this);
}

EpsilonGreedyBanditResource::~EpsilonGreedyBanditResource() {
  AdsClientHelper::RemoveObserver(this);
  catalog_->RemoveObserver(this);
}

// static
SegmentList EpsilonGreedyBanditResource::Get() {
  return GetEpsilonGreedyBanditEligibleSegments();
}

///////////////////////////////////////////////////////////////////////////////

void EpsilonGreedyBanditResource::LoadFromCatalog(const CatalogInfo& catalog) {
  const SegmentList segments = GetSegments(catalog);
  const SegmentList parent_segments = GetParentSegments(segments);

  BLOG(2, "Successfully loaded epsilon greedy bandit segments:");
  for (const auto& segment : parent_segments) {
    BLOG(2, "  " << segment);
  }

  SetEpsilonGreedyBanditEligibleSegments(parent_segments);

  is_initialized_ = true;
}

void EpsilonGreedyBanditResource::MaybeReset() {
  if (IsInitialized() && !DoesRequireResource()) {
    Reset();
  }
}

void EpsilonGreedyBanditResource::Reset() {
  BLOG(1, "Reset epsilon greedy bandit resource");
  ResetEpsilonGreedyBanditEligibleSegments();
  is_initialized_ = false;
}

void EpsilonGreedyBanditResource::OnNotifyPrefDidChange(
    const std::string& path) {
  if (path == hns_rewards::prefs::kEnabled ||
      path == prefs::kOptedInToNotificationAds ||
      path == hns_news::prefs::kHnsNewsOptedIn ||
      path == hns_news::prefs::kNewTabPageShowToday) {
    MaybeReset();
  }
}

void EpsilonGreedyBanditResource::OnDidUpdateCatalog(
    const CatalogInfo& catalog) {
  if (DoesRequireResource()) {
    LoadFromCatalog(catalog);
  }
}

}  // namespace hns_ads
