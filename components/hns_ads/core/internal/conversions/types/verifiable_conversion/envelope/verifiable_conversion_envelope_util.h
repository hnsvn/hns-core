/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_TYPES_VERIFIABLE_CONVERSION_ENVELOPE_VERIFIABLE_CONVERSION_ENVELOPE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_TYPES_VERIFIABLE_CONVERSION_ENVELOPE_VERIFIABLE_CONVERSION_ENVELOPE_UTIL_H_

#include <string>

#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

struct VerifiableConversionEnvelopeInfo;
struct VerifiableConversionInfo;

std::string GetVerifiableConversionEnvelopeAlgorithm();

absl::optional<VerifiableConversionEnvelopeInfo>
SealVerifiableConversionEnvelope(
    const VerifiableConversionInfo& verifiable_conversion);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_TYPES_VERIFIABLE_CONVERSION_ENVELOPE_VERIFIABLE_CONVERSION_ENVELOPE_UTIL_H_
