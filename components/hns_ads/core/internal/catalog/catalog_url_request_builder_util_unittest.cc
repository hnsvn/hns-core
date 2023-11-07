/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_builder_util.h"

#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsCatalogUrlRequestBuilderUtilTest, GetPath) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ("/v9/catalog", BuildCatalogUrlPath());
}

}  // namespace hns_ads
