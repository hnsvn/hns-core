/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/locale/locale_util.h"

#include "hns/components/l10n/common/test/scoped_default_locale.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsLocaleUtilTest, GetLocale) {
  // Arrange
  hns_l10n::test::ScopedDefaultLocale default_locale("en_KY");

  // Act

  // Assert
  EXPECT_EQ("en_KY", GetLocale());
}

}  // namespace hns_ads
