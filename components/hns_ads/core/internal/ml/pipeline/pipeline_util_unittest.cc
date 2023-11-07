/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ml/pipeline/pipeline_util.h"

#include <string>
#include <utility>

#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_file_util.h"
#include "hns/components/hns_ads/core/internal/ml/pipeline/pipeline_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::ml {

namespace {
constexpr char kValidSpamClassificationPipeline[] =
    "ml/pipeline/text_processing/valid_spam_classification.json";
}  // namespace

class HnsAdsPipelineUtilTest : public UnitTestBase {};

TEST_F(HnsAdsPipelineUtilTest, ParsePipelineValueTest) {
  // Arrange
  const absl::optional<std::string> json =
      ReadFileFromTestPathToString(kValidSpamClassificationPipeline);
  ASSERT_TRUE(json);

  base::Value::Dict dict = base::test::ParseJsonDict(*json);

  // Act

  // Assert
  EXPECT_TRUE(pipeline::ParsePipelineValue(std::move(dict)));
}

}  // namespace hns_ads::ml
