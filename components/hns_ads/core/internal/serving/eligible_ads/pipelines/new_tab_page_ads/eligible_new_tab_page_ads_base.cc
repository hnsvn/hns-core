/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/pipelines/new_tab_page_ads/eligible_new_tab_page_ads_base.h"

#include "hns/components/hns_ads/core/internal/targeting/behavioral/anti_targeting/resource/anti_targeting_resource.h"
#include "hns/components/hns_ads/core/internal/targeting/geographical/subdivision/subdivision_targeting.h"

namespace hns_ads {

EligibleNewTabPageAdsBase::EligibleNewTabPageAdsBase(
    const SubdivisionTargeting& subdivision_targeting,
    const AntiTargetingResource& anti_targeting_resource)
    : subdivision_targeting_(subdivision_targeting),
      anti_targeting_resource_(anti_targeting_resource) {}

EligibleNewTabPageAdsBase::~EligibleNewTabPageAdsBase() = default;

}  // namespace hns_ads
