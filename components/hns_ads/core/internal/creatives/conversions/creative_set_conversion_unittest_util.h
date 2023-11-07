/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_CONVERSIONS_CREATIVE_SET_CONVERSION_UNITTEST_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_CONVERSIONS_CREATIVE_SET_CONVERSION_UNITTEST_UTIL_H_

#include <string>

#include "third_party/abseil-cpp/absl/types/optional.h"

namespace base {
class TimeDelta;
}  // namespace base

namespace hns_ads {

struct CreativeSetConversionInfo;

CreativeSetConversionInfo BuildCreativeSetConversionForTesting(
    const std::string& creative_set_id,
    const std::string& url_pattern,
    base::TimeDelta observation_window);
void BuildAndSaveCreativeSetConversionForTesting(
    const std::string& creative_set_id,
    const std::string& url_pattern,
    base::TimeDelta observation_window);

CreativeSetConversionInfo BuildVerifiableCreativeSetConversionForTesting(
    const std::string& creative_set_id,
    const std::string& url_pattern,
    base::TimeDelta observation_window,
    const absl::optional<std::string>& verifiable_advertiser_public_key_base64);
void BuildAndSaveVerifiableCreativeSetConversionForTesting(
    const std::string& creative_set_id,
    const std::string& url_pattern,
    base::TimeDelta observation_window,
    const absl::optional<std::string>& verifiable_advertiser_public_key_base64);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_CONVERSIONS_CREATIVE_SET_CONVERSION_UNITTEST_UTIL_H_
