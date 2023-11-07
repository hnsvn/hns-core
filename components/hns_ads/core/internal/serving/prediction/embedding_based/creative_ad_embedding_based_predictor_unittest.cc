/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/prediction/embedding_based/creative_ad_embedding_based_predictor.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_builder_unittest_util.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_html_event_unittest_util.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_html_events.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCreativeAdEmbeddingBasedPredictorTest : public UnitTestBase {};

TEST_F(HnsAdsCreativeAdEmbeddingBasedPredictorTest, PredictCreativeAd) {
  // Arrange
  CreativeNotificationAdList creative_ads;
  CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/
                                            true);
  creative_ad.embedding = {0.0853, -0.1789, -0.4221};
  creative_ads.push_back(creative_ad);

  const TextEmbeddingHtmlEventInfo text_embedding_html_event =
      BuildTextEmbeddingHtmlEvent(ml::pipeline::BuildTextEmbeddingForTesting());

  const UserModelInfo user_model = BuildUserModelForTesting(
      /*intent_segments*/ {},
      /*latent_interest_segments*/ {},
      /*interest_segments*/ {},
      /*text_embedding_html_events*/ {text_embedding_html_event});

  // Act
  const absl::optional<CreativeNotificationAdInfo> predicted_creative_ad =
      MaybePredictCreativeAd(creative_ads, user_model);

  // Assert
  EXPECT_TRUE(predicted_creative_ad);
}

TEST_F(HnsAdsCreativeAdEmbeddingBasedPredictorTest, DoNotPredictCreativeAd) {
  // Arrange
  CreativeNotificationAdList creative_ads;

  CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad.embedding = {0.0853, -0.1789, -0.4221};
  creative_ads.push_back(creative_ad);

  const UserModelInfo user_model = BuildUserModelForTesting(
      /*intent_segments*/ {},
      /*latent_interest_segments*/ {},
      /*interest_segments*/ {},
      /*text_embedding_html_events*/ {});

  // Act
  const absl::optional<CreativeNotificationAdInfo> predicted_creative_ad =
      MaybePredictCreativeAd(creative_ads, user_model);

  // Assert
  EXPECT_FALSE(predicted_creative_ad);
}

}  // namespace hns_ads
