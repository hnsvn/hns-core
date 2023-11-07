/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/history/category_content_value_util.h"

#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/history/category_content_util.h"
#include "hns/components/hns_ads/core/public/history/category_content_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {
constexpr char kJson[] = R"({"category":"untargeted","optAction":0})";
}  // namespace

class HnsAdsCategoryContentValueUtilTest : public UnitTestBase {};

TEST_F(HnsAdsCategoryContentValueUtilTest, FromValue) {
  // Arrange
  const base::Value::Dict dict = base::test::ParseJsonDict(kJson);

  // Act

  // Assert
  EXPECT_EQ(BuildCategoryContent(kSegment), CategoryContentFromValue(dict));
}

TEST_F(HnsAdsCategoryContentValueUtilTest, ToValue) {
  // Arrange
  const CategoryContentInfo category_content = BuildCategoryContent(kSegment);

  // Act

  // Assert
  EXPECT_EQ(base::test::ParseJsonDict(kJson),
            CategoryContentToValue(category_content));
}

}  // namespace hns_ads
