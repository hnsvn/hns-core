/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/history/history_item_util.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/notification_ad_builder.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_info.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"
#include "hns/components/hns_ads/core/public/history/history_item_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsHistoryItemUtilTest : public UnitTestBase {};

TEST_F(HnsAdsHistoryItemUtilTest, BuildHistoryItem) {
  // Arrange
  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad);

  // Act
  const HistoryItemInfo history_item =
      BuildHistoryItem(ad, ConfirmationType::kViewed, ad.title, ad.body);

  // Assert
  HistoryItemInfo expected_history_item;

  expected_history_item.created_at = Now();
  expected_history_item.ad_content.type = ad.type;
  expected_history_item.ad_content.placement_id = ad.placement_id;
  expected_history_item.ad_content.creative_instance_id =
      ad.creative_instance_id;
  expected_history_item.ad_content.creative_set_id = ad.creative_set_id;
  expected_history_item.ad_content.campaign_id = ad.campaign_id;
  expected_history_item.ad_content.advertiser_id = ad.advertiser_id;
  expected_history_item.ad_content.brand = ad.title;
  expected_history_item.ad_content.brand_info = ad.body;
  expected_history_item.ad_content.brand_display_url = ad.target_url.host();
  expected_history_item.ad_content.brand_url = ad.target_url;
  expected_history_item.ad_content.confirmation_type =
      ConfirmationType::kViewed;
  expected_history_item.ad_content.user_reaction_type =
      mojom::UserReactionType::kNeutral;
  expected_history_item.category_content.category = ad.segment;
  expected_history_item.category_content.user_reaction_type =
      mojom::UserReactionType::kNeutral;

  EXPECT_EQ(expected_history_item, history_item);
}

}  // namespace hns_ads
