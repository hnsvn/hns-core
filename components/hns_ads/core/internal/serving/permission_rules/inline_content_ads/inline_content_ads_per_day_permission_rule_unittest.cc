/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/inline_content_ads/inline_content_ads_per_day_permission_rule.h"

#include "hns/components/hns_ads/core/internal/ads/inline_content_ad/inline_content_ad_feature.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsInlineContentAdsPerDayPermissionRuleTest : public UnitTestBase {
 protected:
  const InlineContentAdsPerDayPermissionRule permission_rule_;
};

TEST_F(HnsAdsInlineContentAdsPerDayPermissionRuleTest,
       ShouldAllowIfThereAreNoAdEvents) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsInlineContentAdsPerDayPermissionRuleTest,
       ShouldAllowIfDoesNotExceedCap) {
  // Arrange

  // Act
  RecordAdEventsForTesting(AdType::kInlineContentAd, ConfirmationType::kServed,
                           /*count*/ kMaximumInlineContentAdsPerDay.Get() - 1);

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsInlineContentAdsPerDayPermissionRuleTest,
       ShouldAllowIfDoesNotExceedCapAfter1Day) {
  // Arrange
  RecordAdEventsForTesting(AdType::kInlineContentAd, ConfirmationType::kServed,
                           /*count*/ kMaximumInlineContentAdsPerDay.Get());

  // Act
  AdvanceClockBy(base::Days(1));

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsInlineContentAdsPerDayPermissionRuleTest,
       ShouldNotAllowIfExceedsCapWithin1Day) {
  // Arrange
  RecordAdEventsForTesting(AdType::kInlineContentAd, ConfirmationType::kServed,
                           /*count*/ kMaximumInlineContentAdsPerDay.Get());

  // Act
  AdvanceClockBy(base::Days(1) - base::Milliseconds(1));

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

}  // namespace hns_ads
