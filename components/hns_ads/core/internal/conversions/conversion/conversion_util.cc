/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_util.h"

#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_info.h"

namespace hns_ads {

namespace {

constexpr char kVerifiableConversion[] = "verifiable conversion";
constexpr char kDefaultConversion[] = "conversion";

}  // namespace

std::string ConversionTypeToString(const ConversionInfo& conversion) {
  return conversion.verifiable ? kVerifiableConversion : kDefaultConversion;
}

}  // namespace hns_ads
