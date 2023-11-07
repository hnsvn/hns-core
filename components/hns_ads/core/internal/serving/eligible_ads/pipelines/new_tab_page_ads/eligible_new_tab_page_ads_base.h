/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PIPELINES_NEW_TAB_PAGE_ADS_ELIGIBLE_NEW_TAB_PAGE_ADS_BASE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PIPELINES_NEW_TAB_PAGE_ADS_ELIGIBLE_NEW_TAB_PAGE_ADS_BASE_H_

#include "base/memory/raw_ref.h"
#include "hns/components/hns_ads/core/internal/creatives/new_tab_page_ads/creative_new_tab_page_ad_info.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/eligible_ads_callback.h"
#include "hns/components/hns_ads/core/public/ad_info.h"

namespace hns_ads {

class AntiTargetingResource;
class SubdivisionTargeting;
struct UserModelInfo;

class EligibleNewTabPageAdsBase {
 public:
  EligibleNewTabPageAdsBase(const EligibleNewTabPageAdsBase&) = delete;
  EligibleNewTabPageAdsBase& operator=(const EligibleNewTabPageAdsBase&) =
      delete;

  EligibleNewTabPageAdsBase(EligibleNewTabPageAdsBase&&) noexcept = delete;
  EligibleNewTabPageAdsBase& operator=(EligibleNewTabPageAdsBase&&) noexcept =
      delete;

  virtual ~EligibleNewTabPageAdsBase();

  virtual void GetForUserModel(
      UserModelInfo user_model,
      EligibleAdsCallback<CreativeNewTabPageAdList> callback) = 0;

  void SetLastServedAd(const AdInfo& ad) { last_served_ad_ = ad; }

 protected:
  EligibleNewTabPageAdsBase(
      const SubdivisionTargeting& subdivision_targeting,
      const AntiTargetingResource& anti_targeting_resource);

  const raw_ref<const SubdivisionTargeting> subdivision_targeting_;
  const raw_ref<const AntiTargetingResource> anti_targeting_resource_;

  AdInfo last_served_ad_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PIPELINES_NEW_TAB_PAGE_ADS_ELIGIBLE_NEW_TAB_PAGE_ADS_BASE_H_
