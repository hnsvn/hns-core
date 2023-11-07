/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/contextual/text_embedding/text_embedding_processor_util.h"

#include <tuple>
#include <vector>

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsTextEmbeddingProcessorUtilTest : public UnitTestBase {};

TEST_F(HnsAdsTextEmbeddingProcessorUtilTest, SanitizeText) {
  // Arrange
  const std::vector<std::tuple<std::string, std::string>> samples = {
      {"test", "test"},
      {" testing   ", "testing"},
      {"test (string) - for 78 unittest 246", "test string for unittest"},
      {"Test this,string - for UNiTTeST", "test this string for unittest"},
      {"Test string, string,... for unittest",
       "test string string for unittest"},
      {"Test string1, string2,... for unittest", "test for unittest"},
      {"321", {}},
      {"<>", {}},
      {" ", {}},
      {{}, {}}};

  // Act
  for (const auto& [text, expected_sanitized_text] : samples) {
    // Assert
    EXPECT_EQ(expected_sanitized_text, SanitizeText(text));
  }
}

}  // namespace hns_ads
