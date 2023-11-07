/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/ads_util.h"

#include "hns/components/l10n/common/test/scoped_default_locale.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsAdsUtilTest, IsSupportedRegion) {
  // Arrange
  hns_l10n::test::ScopedDefaultLocale default_locale("en_US");

  // Act

  // Assert
  EXPECT_TRUE(IsSupportedRegion());
}

TEST(HnsAdsAdsUtilTest, IsUnsupportedRegion) {
  // Arrange
  hns_l10n::test::ScopedDefaultLocale default_locale(/*cuba*/ "en_CU");

  // Act

  // Assert
  EXPECT_FALSE(IsSupportedRegion());
}

}  // namespace hns_ads
