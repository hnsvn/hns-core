/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/history/ad_content_value_util.h"

#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/notification_ad_builder.h"
#include "hns/components/hns_ads/core/internal/history/ad_content_util.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_info.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

constexpr char kTitle[] = "title";
constexpr char kDescription[] = "description";

constexpr char kJson[] =
    R"({"adAction":"view","adType":"ad_notification","advertiserId":"5484a63f-eb99-4ba5-a3b0-8c25d3c0e4b2","brand":"title","brandDisplayUrl":"hns.com","brandInfo":"description","brandUrl":"https://hns.com/","campaignId":"84197fc8-830a-4a8e-8339-7a70c2bfa104","creativeInstanceId":"546fe7b0-5047-4f28-a11c-81f14edcf0f6","creativeSetId":"c2ba3e7d-f688-4bc4-a053-cbe7ac1e6123","flaggedAd":false,"likeAction":0,"placementId":"9bac9ae4-693c-4569-9b3e-300e357780cf","savedAd":false,"segment":"untargeted"})";

}  // namespace

class HnsAdsAdContentValueUtilTest : public UnitTestBase {};

TEST_F(HnsAdsAdContentValueUtilTest, FromValue) {
  // Arrange
  const base::Value::Dict dict = base::test::ParseJsonDict(kJson);

  // Act

  // Assert
  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ false);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad, kPlacementId);
  const AdContentInfo expected_ad_content =
      BuildAdContent(ad, ConfirmationType::kViewed, kTitle, kDescription);
  EXPECT_EQ(expected_ad_content, AdContentFromValue(dict));
}

TEST_F(HnsAdsAdContentValueUtilTest, ToValue) {
  // Arrange
  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ false);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad, kPlacementId);
  const AdContentInfo ad_content =
      BuildAdContent(ad, ConfirmationType::kViewed, kTitle, kDescription);

  // Act

  // Assert
  EXPECT_EQ(base::test::ParseJsonDict(kJson), AdContentToValue(ad_content));
}

}  // namespace hns_ads
