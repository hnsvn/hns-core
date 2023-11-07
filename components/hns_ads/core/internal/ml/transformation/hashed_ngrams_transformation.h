/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_TRANSFORMATION_HASHED_NGRAMS_TRANSFORMATION_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_TRANSFORMATION_HASHED_NGRAMS_TRANSFORMATION_H_

#include <memory>
#include <string>
#include <vector>

#include "hns/components/hns_ads/core/internal/ml/transformation/transformation.h"

namespace hns_ads::ml {

class HashVectorizer;

class HashedNGramsTransformation final : public Transformation {
 public:
  HashedNGramsTransformation();
  HashedNGramsTransformation(int bucket_count,
                             const std::vector<int>& subgrams);

  HashedNGramsTransformation(
      HashedNGramsTransformation&& hashed_ngrams) noexcept;
  HashedNGramsTransformation& operator=(
      HashedNGramsTransformation&& hashed_ngrams) = delete;

  ~HashedNGramsTransformation() override;

  explicit HashedNGramsTransformation(const std::string& parameters);

  std::unique_ptr<Data> Apply(
      const std::unique_ptr<Data>& input_data) const override;

 private:
  std::unique_ptr<HashVectorizer> hash_vectorizer_;
};

}  // namespace hns_ads::ml

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_TRANSFORMATION_HASHED_NGRAMS_TRANSFORMATION_H_
