/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/history/history_item_value_util.h"

#include "base/ranges/algorithm.h"
#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/notification_ad_builder.h"
#include "hns/components/hns_ads/core/internal/history/history_item_util.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_info.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"
#include "hns/components/hns_ads/core/public/history/history_item_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

constexpr char kJson[] =
    R"([{"ad_content":{"adAction":"view","adType":"ad_notification","advertiserId":"5484a63f-eb99-4ba5-a3b0-8c25d3c0e4b2","brand":"Test Ad Title","brandDisplayUrl":"hns.com","brandInfo":"Test Ad Body","brandUrl":"https://hns.com/","campaignId":"84197fc8-830a-4a8e-8339-7a70c2bfa104","creativeInstanceId":"546fe7b0-5047-4f28-a11c-81f14edcf0f6","creativeSetId":"c2ba3e7d-f688-4bc4-a053-cbe7ac1e6123","flaggedAd":false,"likeAction":0,"placementId":"9bac9ae4-693c-4569-9b3e-300e357780cf","savedAd":false,"segment":"untargeted"},"category_content":{"category":"untargeted","optAction":0},"created_at":"13323916800000000"},{"ad_content":{"adAction":"view","adType":"ad_notification","advertiserId":"5484a63f-eb99-4ba5-a3b0-8c25d3c0e4b2","brand":"Test Ad Title","brandDisplayUrl":"hns.com","brandInfo":"Test Ad Body","brandUrl":"https://hns.com/","campaignId":"84197fc8-830a-4a8e-8339-7a70c2bfa104","creativeInstanceId":"546fe7b0-5047-4f28-a11c-81f14edcf0f6","creativeSetId":"c2ba3e7d-f688-4bc4-a053-cbe7ac1e6123","flaggedAd":false,"likeAction":0,"placementId":"9bac9ae4-693c-4569-9b3e-300e357780cf","savedAd":false,"segment":"untargeted"},"category_content":{"category":"untargeted","optAction":0},"created_at":"13323916800000000"}])";

HistoryItemList BuildHistoryItems() {
  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ false);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad, kPlacementId);

  const HistoryItemInfo history_item =
      BuildHistoryItem(ad, ConfirmationType::kViewed, ad.title, ad.body);

  return {history_item, history_item};
}

}  // namespace

class HnsAdsHistoryItemValueUtilTest : public UnitTestBase {};

TEST_F(HnsAdsHistoryItemValueUtilTest, FromValue) {
  // Arrange
  AdvanceClockTo(TimeFromString("22 March 2023", /*is_local*/ false));

  const base::Value::List list = base::test::ParseJsonList(kJson);

  // Act
  const HistoryItemList history_items = HistoryItemsFromValue(list);

  // Assert
  EXPECT_TRUE(base::ranges::equal(BuildHistoryItems(), history_items));
}

TEST_F(HnsAdsHistoryItemValueUtilTest, ToValue) {
  // Arrange
  AdvanceClockTo(TimeFromString("22 March 2023", /*is_local*/ false));

  const HistoryItemList history_items = BuildHistoryItems();

  // Act

  // Assert
  EXPECT_EQ(base::test::ParseJsonList(kJson),
            HistoryItemsToValue(history_items));
}

}  // namespace hns_ads
