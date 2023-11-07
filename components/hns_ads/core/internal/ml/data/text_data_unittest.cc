/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ml/data/text_data.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::ml {

class HnsAdsTextDataTest : public UnitTestBase {};

TEST_F(HnsAdsTextDataTest, TextDataInitialization) {
  // Arrange
  const std::string text = "The quick brown fox jumps over the lazy dog";

  // Act
  const TextData text_data(text);

  // Assert
  EXPECT_EQ(text, text_data.GetText());
}

}  // namespace hns_ads::ml
