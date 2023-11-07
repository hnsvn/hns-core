/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PIPELINES_NEW_TAB_PAGE_ADS_ELIGIBLE_NEW_TAB_PAGE_ADS_FACTORY_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PIPELINES_NEW_TAB_PAGE_ADS_ELIGIBLE_NEW_TAB_PAGE_ADS_FACTORY_H_

#include <memory>

namespace hns_ads {

class AntiTargetingResource;
class EligibleNewTabPageAdsBase;
class SubdivisionTargeting;

class EligibleAdsFactory final {
 public:
  static std::unique_ptr<EligibleNewTabPageAdsBase> Build(
      int version,
      const SubdivisionTargeting& subdivision_targeting,
      const AntiTargetingResource& anti_targeting_resource);
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_PIPELINES_NEW_TAB_PAGE_ADS_ELIGIBLE_NEW_TAB_PAGE_ADS_FACTORY_H_
