/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/priority/priority.h"

#include "base/ranges/algorithm.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsPriorityTest : public UnitTestBase {};

TEST_F(HnsAdsPriorityTest, PrioritizeNoCreativeAds) {
  // Arrange
  const CreativeNotificationAdList creative_ads;

  // Act
  const CreativeNotificationAdList prioritized_creative_ads =
      PrioritizeCreativeAds(creative_ads);

  // Assert
  EXPECT_TRUE(prioritized_creative_ads.empty());
}

TEST_F(HnsAdsPriorityTest, PrioritizeSingleCreativeAd) {
  // Arrange
  CreativeNotificationAdList creative_ads;

  CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad.priority = 1;
  creative_ads.push_back(creative_ad);

  // Act
  const CreativeNotificationAdList prioritized_creative_ads =
      PrioritizeCreativeAds(creative_ads);

  // Assert
  const CreativeNotificationAdList expected_prioritized_creative_ads = {
      creative_ad};
  EXPECT_EQ(expected_prioritized_creative_ads, prioritized_creative_ads);
}

TEST_F(HnsAdsPriorityTest, PrioritizeMultipleCreativeAds) {
  // Arrange
  CreativeNotificationAdList creative_ads;

  CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_1.priority = 1;
  creative_ads.push_back(creative_ad_1);

  CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_2.priority = 2;
  creative_ads.push_back(creative_ad_2);

  CreativeNotificationAdInfo creative_ad_3 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_3.priority = 1;
  creative_ads.push_back(creative_ad_3);

  // Act
  const CreativeNotificationAdList prioritized_creative_ads =
      PrioritizeCreativeAds(creative_ads);

  // Assert
  const CreativeNotificationAdList expected_prioritized_creative_ads = {
      creative_ad_1, creative_ad_3};
  EXPECT_TRUE(base::ranges::equal(expected_prioritized_creative_ads,
                                  prioritized_creative_ads));
}

TEST_F(HnsAdsPriorityTest, DoNotPrioritizeZeroPriorityCreativeAds) {
  // Arrange
  CreativeNotificationAdList creative_ads;

  CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_1.priority = 1;
  creative_ads.push_back(creative_ad_1);

  CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_2.priority = 0;
  creative_ads.push_back(creative_ad_2);

  // Act
  const CreativeNotificationAdList prioritized_creative_ads =
      PrioritizeCreativeAds(creative_ads);

  // Assert
  const CreativeNotificationAdList expected_prioritized_creative_ads = {
      creative_ad_1};
  EXPECT_EQ(expected_prioritized_creative_ads, prioritized_creative_ads);
}

}  // namespace hns_ads
