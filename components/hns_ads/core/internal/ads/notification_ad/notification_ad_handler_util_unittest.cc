/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ads/notification_ad/notification_ad_handler_util.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/notification_ad_builder.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/notification_ad_manager.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

void BuildAndShowNotificationAd() {
  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad);
  ShowNotificationAd(ad);
}

}  // namespace

class HnsAdsNotificationAdUtilTest : public UnitTestBase {};

TEST_F(HnsAdsNotificationAdUtilTest, CanServeIfUserIsActive) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(CanServeIfUserIsActive());
}

TEST_F(HnsAdsNotificationAdUtilTest, DoNotServeIfUserIsActive) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kAndroid);

  // Act

  // Assert
  EXPECT_FALSE(CanServeIfUserIsActive());
}

TEST_F(HnsAdsNotificationAdUtilTest, ShouldServe) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(ShouldServe());
}

TEST_F(HnsAdsNotificationAdUtilTest,
       ShouldNotServeIfOptedOutOfNotificationAds) {
  // Arrange
  DisableNotificationAdsForTesting();

  // Act

  // Assert
  EXPECT_FALSE(ShouldServe());
}

TEST_F(HnsAdsNotificationAdUtilTest, CanServeAtRegularIntervals) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kAndroid);

  // Act

  // Assert
  EXPECT_TRUE(CanServeAtRegularIntervals());
}

TEST_F(HnsAdsNotificationAdUtilTest, DoNotServeAtRegularIntervals) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(CanServeAtRegularIntervals());
}

TEST_F(HnsAdsNotificationAdUtilTest, ShowNotificationAd) {
  // Arrange
  EXPECT_CALL(ads_client_mock_, ShowNotificationAd)
      .WillOnce(::testing::Invoke([](const NotificationAdInfo& ad) {
        // Act

        // Assert
        ASSERT_TRUE(
            NotificationAdManager::GetInstance().Exists(ad.placement_id));
      }));

  BuildAndShowNotificationAd();
}

TEST_F(HnsAdsNotificationAdUtilTest, DismissNotificationAd) {
  // Arrange
  EXPECT_CALL(ads_client_mock_, ShowNotificationAd)
      .WillOnce(::testing::Invoke([](const NotificationAdInfo& ad) {
        ASSERT_TRUE(
            NotificationAdManager::GetInstance().Exists(ad.placement_id));

        // Act
        DismissNotificationAd(ad.placement_id);

        // Assert
        ASSERT_FALSE(
            NotificationAdManager::GetInstance().Exists(ad.placement_id));
      }));

  BuildAndShowNotificationAd();
}

TEST_F(HnsAdsNotificationAdUtilTest, CloseNotificationAd) {
  // Arrange
  EXPECT_CALL(ads_client_mock_, CloseNotificationAd)
      .WillOnce(::testing::Invoke([](const std::string& placement_id) {
        // Act

        // Assert
        ASSERT_FALSE(NotificationAdManager::GetInstance().Exists(placement_id));
      }));

  EXPECT_CALL(ads_client_mock_, ShowNotificationAd)
      .WillOnce(::testing::Invoke([](const NotificationAdInfo& ad) {
        ASSERT_TRUE(
            NotificationAdManager::GetInstance().Exists(ad.placement_id));

        // Act
        CloseNotificationAd(ad.placement_id);

        // Assert
        ASSERT_FALSE(
            NotificationAdManager::GetInstance().Exists(ad.placement_id));
      }));

  BuildAndShowNotificationAd();
}

TEST_F(HnsAdsNotificationAdUtilTest, NotificationAdTimedOut) {
  // Arrange
  EXPECT_CALL(ads_client_mock_, ShowNotificationAd)
      .WillOnce(::testing::Invoke([](const NotificationAdInfo& ad) {
        ASSERT_TRUE(
            NotificationAdManager::GetInstance().Exists(ad.placement_id));

        // Act
        NotificationAdTimedOut(ad.placement_id);

        // Assert
        ASSERT_FALSE(
            NotificationAdManager::GetInstance().Exists(ad.placement_id));
      }));

  BuildAndShowNotificationAd();
}

}  // namespace hns_ads
