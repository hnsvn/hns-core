/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/prediction/embedding_based/voting/creative_ad_embedding_based_predictor_voting.h"

#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_html_event_unittest_util.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_html_events.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsCreativeAdEmbeddingBasedPredictorVotingTest,
     ComputeCreativeAdVoteRegistryForTextEmbeddingHtmlEvents) {
  // Arrange
  CreativeNotificationAdList creative_ads;

  CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_1.embedding = {-0.0853, -0.1789, -0.4221};
  creative_ads.push_back(creative_ad_1);

  CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_2.embedding = {0.0853, -0.1789, 0.4221};
  creative_ads.push_back(creative_ad_2);

  CreativeNotificationAdInfo creative_ad_3 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_3.embedding = {-0.0853, 0.1789, -0.4221};
  creative_ads.push_back(creative_ad_3);

  CreativeNotificationAdInfo creative_ad_4 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_4.creative_instance_id = creative_ad_2.creative_instance_id;
  creative_ad_4.embedding = {0.0853, -0.1789, 0.4221};
  creative_ads.push_back(creative_ad_4);

  TextEmbeddingHtmlEventList text_embedding_html_events;

  const TextEmbeddingHtmlEventInfo text_embedding_html_event_1 =
      BuildTextEmbeddingHtmlEvent(ml::pipeline::BuildTextEmbeddingForTesting());
  text_embedding_html_events.push_back(text_embedding_html_event_1);

  const TextEmbeddingHtmlEventInfo text_embedding_html_event_2 =
      BuildTextEmbeddingHtmlEvent(ml::pipeline::BuildTextEmbeddingForTesting());
  text_embedding_html_events.push_back(text_embedding_html_event_2);

  // Act
  const std::vector<int> creative_ad_vote_registry =
      ComputeCreativeAdVoteRegistryForTextEmbeddingHtmlEvents(
          creative_ads, text_embedding_html_events);

  // Assert
  const std::vector<int> expected_creative_ad_vote_registry = {0, 2, 0, 2};
  EXPECT_EQ(expected_creative_ad_vote_registry, creative_ad_vote_registry);
}

TEST(HnsAdsCreativeAdEmbeddingBasedPredictorVotingTest,
     ComputeCreativeAdVoteRegistryForSameTextEmbeddingHtmlEvents) {
  // Arrange
  CreativeNotificationAdList creative_ads;

  CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_1.embedding = {0.0853, -0.1789, 0.4221};
  creative_ads.push_back(creative_ad_1);

  CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_2.embedding = {0.0853, -0.1789, 0.4221};
  creative_ads.push_back(creative_ad_2);

  CreativeNotificationAdInfo creative_ad_3 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_3.embedding = {0.0853, -0.1789, 0.4221};
  creative_ads.push_back(creative_ad_3);

  TextEmbeddingHtmlEventList text_embedding_html_events;

  const TextEmbeddingHtmlEventInfo text_embedding_html_event_1 =
      BuildTextEmbeddingHtmlEvent(ml::pipeline::BuildTextEmbeddingForTesting());
  text_embedding_html_events.push_back(text_embedding_html_event_1);

  const TextEmbeddingHtmlEventInfo text_embedding_html_event_2 =
      BuildTextEmbeddingHtmlEvent(ml::pipeline::BuildTextEmbeddingForTesting());
  text_embedding_html_events.push_back(text_embedding_html_event_2);

  // Act
  const std::vector<int> creative_ad_vote_registry =
      ComputeCreativeAdVoteRegistryForTextEmbeddingHtmlEvents(
          creative_ads, text_embedding_html_events);

  // Assert
  const std::vector<int> expected_creative_ad_vote_registry = {2, 2, 2};
  EXPECT_EQ(expected_creative_ad_vote_registry, creative_ad_vote_registry);
}

TEST(HnsAdsCreativeAdEmbeddingBasedPredictorVotingTest,
     ComputeCreativeAdVoteRegistryForNoTextEmbeddingHtmlEvents) {
  // Arrange
  CreativeNotificationAdList creative_ads;

  CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_1.embedding = {-0.0853, -0.1789, -0.4221};
  creative_ads.push_back(creative_ad_1);

  CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_2.embedding = {0.0853, -0.1789, 0.4221};
  creative_ads.push_back(creative_ad_2);

  CreativeNotificationAdInfo creative_ad_3 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_3.embedding = {-0.0853, 0.1789, -0.4221};
  creative_ads.push_back(creative_ad_3);

  CreativeNotificationAdInfo creative_ad_4 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_4.creative_instance_id = creative_ad_2.creative_instance_id;
  creative_ad_4.embedding = {0.0853, -0.1789, 0.4221};
  creative_ads.push_back(creative_ad_4);

  const TextEmbeddingHtmlEventList text_embedding_html_events;

  // Act
  const std::vector<int> creative_ad_vote_registry =
      ComputeCreativeAdVoteRegistryForTextEmbeddingHtmlEvents(
          creative_ads, text_embedding_html_events);

  // Assert
  const std::vector<int> expected_creative_ad_vote_registry = {0, 0, 0, 0};
  EXPECT_EQ(expected_creative_ad_vote_registry, creative_ad_vote_registry);
}

}  // namespace hns_ads
