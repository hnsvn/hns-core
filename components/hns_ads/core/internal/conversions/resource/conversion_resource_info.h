/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_RESOURCE_CONVERSION_RESOURCE_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_RESOURCE_CONVERSION_RESOURCE_INFO_H_

#include <string>

#include "base/types/expected.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/internal/conversions/resource/conversion_resource_id_pattern_info.h"

namespace hns_ads {

struct ConversionResourceInfo final {
  ConversionResourceInfo();

  ConversionResourceInfo(const ConversionResourceInfo&) = delete;
  ConversionResourceInfo& operator=(const ConversionResourceInfo&) = delete;

  ConversionResourceInfo(ConversionResourceInfo&& other) noexcept;
  ConversionResourceInfo& operator=(ConversionResourceInfo&& other) noexcept;

  ~ConversionResourceInfo();

  static base::expected<ConversionResourceInfo, std::string> CreateFromValue(
      base::Value::Dict dict);

  int version = 0;
  ConversionResourceIdPatternMap id_patterns;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_RESOURCE_CONVERSION_RESOURCE_INFO_H_
