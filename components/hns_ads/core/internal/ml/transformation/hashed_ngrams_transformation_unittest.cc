/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ml/transformation/hashed_ngrams_transformation.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/ml/data/text_data.h"
#include "hns/components/hns_ads/core/internal/ml/data/vector_data.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::ml {

class HnsAdsHashedNGramsTransformationTest : public UnitTestBase {};

TEST_F(HnsAdsHashedNGramsTransformationTest, WrongInputDataTest) {
  // Arrange
  const std::unique_ptr<Data> vector_data =
      std::make_unique<VectorData>(std::vector<float>(1.0F));

  const HashedNGramsTransformation hashed_ngrams;

  // Act
  const std::unique_ptr<Data> output_data = hashed_ngrams.Apply(vector_data);

  // Assert
  EXPECT_FALSE(output_data.get());
}

TEST_F(HnsAdsHashedNGramsTransformationTest, HashingTest) {
  // Arrange
  constexpr size_t kDefaultBucketCount = 10'000;
  constexpr char kTestString[] = "tiny";
  const std::unique_ptr<Data> text_data =
      std::make_unique<TextData>(kTestString);

  const HashedNGramsTransformation hashed_ngrams;

  // Act
  const std::unique_ptr<Data> hashed_data = hashed_ngrams.Apply(text_data);

  ASSERT_EQ(hashed_data->GetType(), DataType::kVector);

  const VectorData* const hashed_vector_data =
      static_cast<VectorData*>(hashed_data.get());

  // Assert
  // 10000 is the default size
  ASSERT_EQ(kDefaultBucketCount, hashed_vector_data->GetDimensionCount());

  // Hashes for [t, i, n, y, ti, in, ny, tin, iny, tiny] -- 10 in total
  EXPECT_EQ(10U, hashed_vector_data->GetData().size());
}

TEST_F(HnsAdsHashedNGramsTransformationTest, CustomHashingTest) {
  // Arrange
  constexpr size_t kHashBucketCount = 3;
  constexpr char kTestString[] = "tiny";
  const std::unique_ptr<Data> text_data =
      std::make_unique<TextData>(kTestString);

  const HashedNGramsTransformation hashed_ngrams(kHashBucketCount,
                                                 std::vector<int>{1, 2, 3});

  // Act
  const std::unique_ptr<Data> hashed_data = hashed_ngrams.Apply(text_data);

  ASSERT_EQ(DataType::kVector, hashed_data->GetType());

  const VectorData* const hashed_vector_data =
      static_cast<VectorData*>(hashed_data.get());

  // Assert
  ASSERT_EQ(kHashBucketCount, hashed_vector_data->GetDimensionCount());
  EXPECT_EQ(kHashBucketCount, hashed_vector_data->GetData().size());
}

}  // namespace hns_ads::ml
