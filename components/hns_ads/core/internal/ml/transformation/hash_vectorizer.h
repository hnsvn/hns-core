/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_TRANSFORMATION_HASH_VECTORIZER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_TRANSFORMATION_HASH_VECTORIZER_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace hns_ads::ml {

class HashVectorizer final {
 public:
  HashVectorizer();
  HashVectorizer(int bucket_count, const std::vector<int>& subgrams);

  HashVectorizer(const HashVectorizer&) = delete;
  HashVectorizer& operator=(const HashVectorizer&) = delete;

  HashVectorizer(HashVectorizer&&) noexcept = delete;
  HashVectorizer& operator=(HashVectorizer&&) noexcept = delete;

  ~HashVectorizer();

  std::map<uint32_t, double> GetFrequencies(const std::string& html) const;

  std::vector<uint32_t> GetSubstringSizes() const;

  int GetBucketCount() const;

 private:
  std::vector<uint32_t> substring_sizes_;
  int bucket_count_;
};

}  // namespace hns_ads::ml

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ML_TRANSFORMATION_HASH_VECTORIZER_H_
