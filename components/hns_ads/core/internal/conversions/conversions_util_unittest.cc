/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/conversions_util.h"

#include "hns/components/hns_ads/core/internal/ads/ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_builder.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"
#include "hns/components/hns_ads/core/public/ad_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsConversionsUtilTest : public UnitTestBase {};

TEST_F(HnsAdsConversionsUtilTest, CanConvertInlineContentAdViewedEvent) {
  // Arrange
  const AdInfo ad =
      BuildAdForTesting(AdType::kInlineContentAd, /*should_use_random_uuids*/
                        true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now());

  // Assert
  EXPECT_TRUE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CanConvertInlineContentAdClickedEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kInlineContentAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kClicked, /*created_at*/ Now());

  // Assert
  EXPECT_TRUE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CannotConvertInlineContentAdEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kInlineContentAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kServed, /*created_at*/ Now());

  // Assert
  EXPECT_FALSE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest,
       CannotConvertInlineContentAdEventIfHnsNewsAdsAreDisabled) {
  // Arrange
  DisableHnsNewsAdsForTesting();

  const AdInfo ad = BuildAdForTesting(AdType::kInlineContentAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now());

  // Assert
  EXPECT_FALSE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CanConvertPromotedContentAdViewedEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kPromotedContentAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now());

  // Assert
  EXPECT_TRUE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CanConvertPromotedContentAdClickedEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kPromotedContentAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kClicked, /*created_at*/ Now());

  // Assert
  EXPECT_TRUE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CannotConvertPromotedContentAdEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kPromotedContentAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kServed, /*created_at*/ Now());

  // Assert
  EXPECT_FALSE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest,
       CannotConvertPromotedContentAdEventIfHnsNewsAdsAreDisabled) {
  // Arrange
  DisableHnsNewsAdsForTesting();

  const AdInfo ad = BuildAdForTesting(AdType::kPromotedContentAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now());

  // Assert
  EXPECT_FALSE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CanConvertNotificationAdViewedEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now());

  // Assert
  EXPECT_TRUE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CanConvertNotificationAdClickedEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kClicked, /*created_at*/ Now());

  // Assert
  EXPECT_TRUE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CannotConvertNotificationAdEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kServed, /*created_at*/ Now());

  // Assert
  EXPECT_FALSE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest,
       CannotConvertNotificationAdEventIfOptedOutOfNotificationAds) {
  // Arrange
  DisableNotificationAdsForTesting();

  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now());

  // Assert
  EXPECT_FALSE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CanConvertNewTabPageAdViewedEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNewTabPageAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now());

  // Assert
  EXPECT_TRUE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CanConvertNewTabPageAdClickedEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNewTabPageAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kClicked, /*created_at*/ Now());

  // Assert
  EXPECT_TRUE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CannotConvertNewTabPageAdEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNewTabPageAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kServed, /*created_at*/ Now());

  // Assert
  EXPECT_FALSE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest,
       CannotConvertNewTabPageAdEventIfNewTabPageAdsAreDisabled) {
  // Arrange
  DisableNewTabPageAdsForTesting();

  const AdInfo ad = BuildAdForTesting(AdType::kNewTabPageAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now());

  // Assert
  EXPECT_FALSE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CanConvertSearchResultAdViewedEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kSearchResultAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now());

  // Assert
  EXPECT_TRUE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CanConvertSearchResultAdClickedEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kSearchResultAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kClicked, /*created_at*/ Now());

  // Assert
  EXPECT_TRUE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, CannotConvertSearchResultAdEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kSearchResultAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kServed, /*created_at*/ Now());

  // Assert
  EXPECT_FALSE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest,
       CanConvertSearchResultAdEventIfAdsAreDisabled) {
  // Arrange
  DisableHnsNewsAdsForTesting();
  DisableNotificationAdsForTesting();
  DisableNewTabPageAdsForTesting();

  const AdInfo ad = BuildAdForTesting(AdType::kNewTabPageAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now());

  // Assert
  EXPECT_FALSE(CanConvertAdEvent(ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, HasObservationWindowForAdEventExpired) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now());

  AdvanceClockBy(base::Days(1) + base::Milliseconds(1));

  // Assert
  EXPECT_TRUE(HasObservationWindowForAdEventExpired(
      /*observation_window*/ base::Days(1), ad_event));
}

TEST_F(HnsAdsConversionsUtilTest, HasObservationWindowForAdEventNotExpired) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  // Act
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now());

  AdvanceClockBy(base::Days(1));

  // Assert
  EXPECT_FALSE(HasObservationWindowForAdEventExpired(
      /*observation_window*/ base::Days(1), ad_event));
}

}  // namespace hns_ads
