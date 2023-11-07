/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/search_result_ads/search_result_ads_per_day_permission_rule.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"
#include "hns/components/hns_ads/core/public/feature/search_result_ad_feature.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsSearchResultAdsPerDayPermissionRuleTest : public UnitTestBase {
 protected:
  const SearchResultAdsPerDayPermissionRule permission_rule_;
};

TEST_F(HnsAdsSearchResultAdsPerDayPermissionRuleTest,
       ShouldAllowIfThereAreNoAdEvents) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsSearchResultAdsPerDayPermissionRuleTest,
       ShouldAllowIfDoesNotExceedCap) {
  // Arrange

  // Act
  RecordAdEventsForTesting(AdType::kSearchResultAd, ConfirmationType::kServed,
                           /*count*/ kMaximumSearchResultAdsPerDay.Get() - 1);

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsSearchResultAdsPerDayPermissionRuleTest,
       ShouldAllowIfDoesNotExceedCapAfter1Day) {
  // Arrange
  RecordAdEventsForTesting(AdType::kSearchResultAd, ConfirmationType::kServed,
                           /*count*/ kMaximumSearchResultAdsPerDay.Get());

  // Act
  AdvanceClockBy(base::Days(1));

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsSearchResultAdsPerDayPermissionRuleTest,
       ShouldNotAllowIfExceedsCapWithin1Day) {
  // Arrange
  RecordAdEventsForTesting(AdType::kSearchResultAd, ConfirmationType::kServed,
                           /*count*/ kMaximumSearchResultAdsPerDay.Get());

  // Act
  AdvanceClockBy(base::Days(1) - base::Milliseconds(1));

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

}  // namespace hns_ads
