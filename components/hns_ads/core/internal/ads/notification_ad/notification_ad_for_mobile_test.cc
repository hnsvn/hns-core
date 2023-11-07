/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/test/mock_callback.h"
#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/notification_ad_manager.h"
#include "hns/components/hns_ads/core/internal/history/history_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/notification_ad_serving_util.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rules_unittest_util.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_info.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"
#include "net/http/http_status_code.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsNotificationAdForMobileIntegrationTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUpForTesting(/*is_integration_test*/ true);
  }

  void SetUpMocks() override {
    MockPlatformHelper(platform_helper_mock_, PlatformType::kAndroid);

    const URLResponseMap url_responses = {
        {BuildCatalogUrlPath(),
         {{net::HTTP_OK,
           /*response_body*/ "/catalog_with_notification_ad.json"}}}};
    MockUrlResponses(ads_client_mock_, url_responses);
  }

  void ServeNextAd() {
    ASSERT_TRUE(ShouldServeAdsAtRegularIntervals());
    FastForwardClockTo(ServeAdAt());
  }

  void ServeAd() {
    NotifyUserDidBecomeActive(/*idle_time*/ base::TimeDelta::Min(),
                              /*screen_was_locked*/ false);
  }
};

TEST_F(HnsAdsNotificationAdForMobileIntegrationTest,
       ServeWhenUserBecomesActive) {
  // Arrange
  ForcePermissionRulesForTesting();

  EXPECT_CALL(ads_client_mock_, ShowNotificationAd)
      .WillOnce(::testing::Invoke([](const NotificationAdInfo& ad) {
        // Assert
        ASSERT_TRUE(
            NotificationAdManager::GetInstance().Exists(ad.placement_id));
      }));

  EXPECT_CALL(ads_client_mock_, RecordP2AEvents);

  EXPECT_CALL(ads_client_mock_, AddTrainingSample).Times(0);

  // Act
  ServeNextAd();
}

TEST_F(HnsAdsNotificationAdForMobileIntegrationTest,
       DoNotServeWhenUserBecomesActive) {
  // Arrange

  // Assert
  EXPECT_CALL(ads_client_mock_, ShowNotificationAd).Times(0);

  EXPECT_CALL(ads_client_mock_, RecordP2AEvents).Times(0);

  EXPECT_CALL(ads_client_mock_, AddTrainingSample).Times(0);

  // Act
  ServeAd();
}

TEST_F(HnsAdsNotificationAdForMobileIntegrationTest,
       ServeAtRegularIntervals) {
  // Arrange

  // Assert

  // Act
  EXPECT_TRUE(ShouldServeAdsAtRegularIntervals());
}

TEST_F(HnsAdsNotificationAdForMobileIntegrationTest, TriggerViewedEvent) {
  // Arrange
  ForcePermissionRulesForTesting();

  EXPECT_CALL(ads_client_mock_, ShowNotificationAd)
      .WillOnce(::testing::Invoke([=](const NotificationAdInfo& ad) {
        ASSERT_TRUE(
            NotificationAdManager::GetInstance().Exists(ad.placement_id));

        base::MockCallback<TriggerAdEventCallback> callback;
        EXPECT_CALL(callback, Run).WillOnce([&ad](const bool success) {
          // Assert
          EXPECT_TRUE(success);
          ASSERT_TRUE(
              NotificationAdManager::GetInstance().Exists(ad.placement_id));
          EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kNotificationAd,
                                                  ConfirmationType::kServed));
          EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kNotificationAd,
                                                  ConfirmationType::kViewed));
          EXPECT_EQ(1U, GetHistoryItemCountForTesting());
          EXPECT_EQ(1U, GetTransactionCountForTesting());
        });

        EXPECT_CALL(ads_client_mock_, AddTrainingSample).Times(0);

        // Act
        GetAds().TriggerNotificationAdEvent(
            ad.placement_id, mojom::NotificationAdEventType::kViewed,
            callback.Get());
      }));

  ServeNextAd();
}

TEST_F(HnsAdsNotificationAdForMobileIntegrationTest, TriggerClickedEvent) {
  // Arrange
  ForcePermissionRulesForTesting();

  EXPECT_CALL(ads_client_mock_, ShowNotificationAd)
      .WillOnce(::testing::Invoke([=](const NotificationAdInfo& ad) {
        ASSERT_TRUE(
            NotificationAdManager::GetInstance().Exists(ad.placement_id));
        EXPECT_CALL(ads_client_mock_, CloseNotificationAd(ad.placement_id));

        base::MockCallback<TriggerAdEventCallback> callback;
        EXPECT_CALL(callback, Run).WillOnce([](const bool success) {
          // Assert
          EXPECT_TRUE(success);
          EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kNotificationAd,
                                                  ConfirmationType::kClicked));
          EXPECT_EQ(1U, GetHistoryItemCountForTesting());
          EXPECT_EQ(1U, GetTransactionCountForTesting());
        });

        EXPECT_CALL(ads_client_mock_, AddTrainingSample);

        // Act
        GetAds().TriggerNotificationAdEvent(
            ad.placement_id, mojom::NotificationAdEventType::kClicked,
            callback.Get());
      }));

  ServeNextAd();
}

TEST_F(HnsAdsNotificationAdForMobileIntegrationTest, TriggerDismissedEvent) {
  // Arrange
  ForcePermissionRulesForTesting();

  EXPECT_CALL(ads_client_mock_, ShowNotificationAd)
      .WillOnce(::testing::Invoke([=](const NotificationAdInfo& ad) {
        ASSERT_TRUE(
            NotificationAdManager::GetInstance().Exists(ad.placement_id));

        base::MockCallback<TriggerAdEventCallback> callback;
        EXPECT_CALL(callback, Run).WillOnce([&ad](const bool success) {
          // Assert
          EXPECT_TRUE(success);
          EXPECT_FALSE(
              NotificationAdManager::GetInstance().Exists(ad.placement_id));
          EXPECT_EQ(1U,
                    GetAdEventCountForTesting(AdType::kNotificationAd,
                                              ConfirmationType::kDismissed));
          EXPECT_EQ(1U, GetHistoryItemCountForTesting());
          EXPECT_EQ(1U, GetTransactionCountForTesting());
        });

        EXPECT_CALL(ads_client_mock_, AddTrainingSample);

        // Act
        GetAds().TriggerNotificationAdEvent(
            ad.placement_id, mojom::NotificationAdEventType::kDismissed,
            callback.Get());
      }));

  ServeNextAd();
}

TEST_F(HnsAdsNotificationAdForMobileIntegrationTest, TriggerTimedOutEvent) {
  // Arrange
  ForcePermissionRulesForTesting();

  EXPECT_CALL(ads_client_mock_, ShowNotificationAd)
      .WillOnce(::testing::Invoke([=](const NotificationAdInfo& ad) {
        ASSERT_TRUE(
            NotificationAdManager::GetInstance().Exists(ad.placement_id));

        base::MockCallback<TriggerAdEventCallback> callback;
        EXPECT_CALL(callback, Run).WillOnce([&ad](const bool success) {
          // Assert
          EXPECT_TRUE(success);
          EXPECT_FALSE(
              NotificationAdManager::GetInstance().Exists(ad.placement_id));
          EXPECT_EQ(0U, GetHistoryItemCountForTesting());
          EXPECT_EQ(0U, GetTransactionCountForTesting());
        });

        EXPECT_CALL(ads_client_mock_, AddTrainingSample);

        // Act
        GetAds().TriggerNotificationAdEvent(
            ad.placement_id, mojom::NotificationAdEventType::kTimedOut,
            callback.Get());
      }));

  ServeNextAd();
}

}  // namespace hns_ads
