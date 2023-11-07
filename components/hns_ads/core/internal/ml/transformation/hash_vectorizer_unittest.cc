/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ml/transformation/hash_vectorizer.h"

#include "base/test/values_test_util.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_file_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::ml {

namespace {

constexpr char kHashCheck[] = "ml/hash_vectorizer/hashing_validation.json";

void RunHashingExtractorTestCase(const std::string& test_case_name) {
  // Arrange
  constexpr double kTolerance = 1e-7;

  // Act
  const base::Value::Dict root =
      base::test::ParseJsonDictFromFile(GetTestPath().AppendASCII(kHashCheck));

  const base::Value::Dict* const test_case_name_dict =
      root.FindDict(test_case_name);
  ASSERT_TRUE(test_case_name_dict);

  const std::string* const input = test_case_name_dict->FindString("input");
  ASSERT_TRUE(input);

  const base::Value::List* const idx_list =
      test_case_name_dict->FindList("idx");
  ASSERT_TRUE(idx_list);

  const base::Value::List* const count_list =
      test_case_name_dict->FindList("count");
  ASSERT_TRUE(count_list);

  const HashVectorizer vectorizer;
  const std::map<unsigned, double> frequencies =
      vectorizer.GetFrequencies(*input);
  ASSERT_EQ(frequencies.size(), idx_list->size());

  // Assert
  for (size_t i = 0; i < frequencies.size(); ++i) {
    const base::Value& idx = (*idx_list)[i];
    ASSERT_TRUE(idx.is_int());

    const base::Value& count = (*count_list)[i];
    ASSERT_TRUE(count.is_int());

    EXPECT_LT(count.GetInt() - frequencies.at(idx.GetInt()), kTolerance);
  }
}

}  // namespace

class HnsAdsHashVectorizerTest : public UnitTestBase {};

TEST_F(HnsAdsHashVectorizerTest, ValidJsonScheme) {
  // Arrange
  const base::Value::Dict dict = base::test::ParseJsonDict(
      R"({"test":{"foo":true,"bar":3.14,"baz":"qux","quux":"corge"},"list":["grault","garply"]})");

  // Act

  // Assert
  EXPECT_TRUE(dict.FindDict("test"));
  EXPECT_TRUE(dict.FindList("list"));
}

TEST_F(HnsAdsHashVectorizerTest, EmptyText) {
  RunHashingExtractorTestCase("empty");
}

TEST_F(HnsAdsHashVectorizerTest, ShortText) {
  RunHashingExtractorTestCase("tiny");
}

TEST_F(HnsAdsHashVectorizerTest, EnglishText) {
  RunHashingExtractorTestCase("english");
}

TEST_F(HnsAdsHashVectorizerTest, GreekText) {
  RunHashingExtractorTestCase("greek");
}

TEST_F(HnsAdsHashVectorizerTest, JapaneseText) {
  RunHashingExtractorTestCase("japanese");
}

}  // namespace hns_ads::ml
