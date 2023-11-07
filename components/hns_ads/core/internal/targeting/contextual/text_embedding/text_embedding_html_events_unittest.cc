/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_html_events.h"

#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/ml/pipeline/text_processing/embedding_info.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_feature.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_html_event_info.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_html_event_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsTextEmbeddingHtmlEventsTest : public UnitTestBase {};

TEST_F(HnsAdsTextEmbeddingHtmlEventsTest, BuildEvent) {
  // Arrange
  const ml::pipeline::TextEmbeddingInfo text_embedding =
      ml::pipeline::BuildTextEmbeddingForTesting();

  // Act
  const TextEmbeddingHtmlEventInfo text_embedding_html_event =
      BuildTextEmbeddingHtmlEvent(text_embedding);

  // Assert
  EXPECT_EQ(text_embedding.locale, text_embedding_html_event.locale);
  EXPECT_EQ(text_embedding.hashed_text_base64,
            text_embedding_html_event.hashed_text_base64);
  EXPECT_EQ(text_embedding.embedding, text_embedding_html_event.embedding);
}

TEST_F(HnsAdsTextEmbeddingHtmlEventsTest, LogEvent) {
  // Arrange
  const ml::pipeline::TextEmbeddingInfo text_embedding =
      ml::pipeline::BuildTextEmbeddingForTesting();

  // Act
  LogTextEmbeddingHtmlEvent(
      BuildTextEmbeddingHtmlEvent(text_embedding),
      base::BindOnce([](const bool success) { ASSERT_TRUE(success); }));

  GetTextEmbeddingHtmlEventsFromDatabase(base::BindOnce(
      [](const ml::pipeline::TextEmbeddingInfo& text_embedding,
         const bool success,
         const TextEmbeddingHtmlEventList& text_embedding_html_events) {
        ASSERT_TRUE(!text_embedding_html_events.empty());
        ASSERT_TRUE(success);

        // Assert
        EXPECT_EQ(text_embedding.hashed_text_base64,
                  text_embedding_html_events.front().hashed_text_base64);
      },
      text_embedding));
}

TEST_F(HnsAdsTextEmbeddingHtmlEventsTest, PurgeEvents) {
  // Arrange
  for (int i = 0; i < kTextEmbeddingHistorySize.Get() + 4; i++) {
    const ml::pipeline::TextEmbeddingInfo text_embedding =
        ml::pipeline::BuildTextEmbeddingForTesting();
    LogTextEmbeddingHtmlEvent(
        BuildTextEmbeddingHtmlEvent(text_embedding),
        base::BindOnce([](const bool success) { ASSERT_TRUE(success); }));
  }

  // Act
  PurgeStaleTextEmbeddingHtmlEvents(
      base::BindOnce([](const bool success) { ASSERT_TRUE(success); }));

  // Assert
  GetTextEmbeddingHtmlEventsFromDatabase(base::BindOnce(
      [](const bool success,
         const TextEmbeddingHtmlEventList& text_embedding_html_events) {
        ASSERT_TRUE(success);

        EXPECT_LE(static_cast<int>(text_embedding_html_events.size()),
                  kTextEmbeddingHistorySize.Get());
      }));
}

}  // namespace hns_ads
