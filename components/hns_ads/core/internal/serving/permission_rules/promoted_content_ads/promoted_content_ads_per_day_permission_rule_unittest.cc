/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/promoted_content_ads/promoted_content_ads_per_day_permission_rule.h"

#include "hns/components/hns_ads/core/internal/ads/promoted_content_ad/promoted_content_ad_feature.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsPromotedContentAdsPerDayPermissionRuleTest : public UnitTestBase {
 protected:
  const PromotedContentAdsPerDayPermissionRule permission_rule_;
};

TEST_F(HnsAdsPromotedContentAdsPerDayPermissionRuleTest,
       ShouldAllowIfThereAreNoAdEvents) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsPromotedContentAdsPerDayPermissionRuleTest,
       ShouldAllowIfDoesNotExceedCap) {
  // Arrange

  // Act
  RecordAdEventsForTesting(
      AdType::kPromotedContentAd, ConfirmationType::kServed,
      /*count*/ kMaximumPromotedContentAdsPerDay.Get() - 1);

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsPromotedContentAdsPerDayPermissionRuleTest,
       ShouldAllowIfDoesNotExceedCapAfter1Day) {
  // Arrange
  RecordAdEventsForTesting(AdType::kPromotedContentAd,
                           ConfirmationType::kServed,
                           /*count*/ kMaximumPromotedContentAdsPerDay.Get());

  // Act
  AdvanceClockBy(base::Days(1));

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsPromotedContentAdsPerDayPermissionRuleTest,
       ShouldNotAllowIfExceedsCapWithin1Day) {
  // Arrange
  RecordAdEventsForTesting(AdType::kPromotedContentAd,
                           ConfirmationType::kServed,
                           /*count*/ kMaximumPromotedContentAdsPerDay.Get());

  // Act
  AdvanceClockBy(base::Days(1) - base::Milliseconds(1));

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

}  // namespace hns_ads
