/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_TYPES_VERIFIABLE_CONVERSION_ENVELOPE_VERIFIABLE_CONVERSION_ENVELOPE_UNITTEST_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_TYPES_VERIFIABLE_CONVERSION_ENVELOPE_VERIFIABLE_CONVERSION_ENVELOPE_UNITTEST_UTIL_H_

#include <string>

#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

struct VerifiableConversionEnvelopeInfo;

absl::optional<std::string> OpenVerifiableConversionEnvelopeForTesting(
    const VerifiableConversionEnvelopeInfo& verifiable_conversion_envelope,
    const std::string& advertiser_secret_key_base64);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_TYPES_VERIFIABLE_CONVERSION_ENVELOPE_VERIFIABLE_CONVERSION_ENVELOPE_UNITTEST_UTIL_H_
