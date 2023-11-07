/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/new_tab_page_ads/new_tab_page_ads_per_hour_permission_rule.h"

#include "hns/components/hns_ads/core/internal/ads/new_tab_page_ad/new_tab_page_ad_feature.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsNewTabPageAdsPerHourPermissionRuleTest : public UnitTestBase {
 protected:
  const NewTabPageAdsPerHourPermissionRule permission_rule_;
};

TEST_F(HnsAdsNewTabPageAdsPerHourPermissionRuleTest,
       ShouldAllowIfThereAreNoAdEvents) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsNewTabPageAdsPerHourPermissionRuleTest,
       ShouldAllowIfDoesNotExceedCap) {
  // Arrange
  RecordAdEventsForTesting(AdType::kNewTabPageAd, ConfirmationType::kServed,
                           /*count*/ kMaximumNewTabPageAdsPerHour.Get() - 1);

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsNewTabPageAdsPerHourPermissionRuleTest,
       ShouldAllowIfDoesNotExceedCapAfter1Hour) {
  // Arrange
  RecordAdEventsForTesting(AdType::kNewTabPageAd, ConfirmationType::kServed,
                           /*count*/ kMaximumNewTabPageAdsPerHour.Get());

  // Act
  AdvanceClockBy(base::Hours(1));

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsNewTabPageAdsPerHourPermissionRuleTest,
       ShouldNotAllowIfExceedsCapWithin1Hour) {
  // Arrange
  RecordAdEventsForTesting(AdType::kNewTabPageAd, ConfirmationType::kServed,
                           /*count*/ kMaximumNewTabPageAdsPerHour.Get());

  // Act
  AdvanceClockBy(base::Hours(1) - base::Milliseconds(1));

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

}  // namespace hns_ads
