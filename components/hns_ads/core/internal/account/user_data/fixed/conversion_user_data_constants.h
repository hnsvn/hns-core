/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_USER_DATA_FIXED_CONVERSION_USER_DATA_CONSTANTS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_USER_DATA_FIXED_CONVERSION_USER_DATA_CONSTANTS_H_

namespace hns_ads {

constexpr char kConversionKey[] = "conversion";

constexpr char kConversionActionTypeKey[] = "action";

constexpr char kVerifiableConversionEnvelopeKey[] = "envelope";
constexpr char kVerifiableConversionEnvelopeAlgorithmKey[] = "alg";
constexpr char kVerifiableConversionEnvelopeCipherTextKey[] = "ciphertext";
constexpr char kVerifiableConversionEnvelopeEphemeralPublicKeyKey[] = "epk";
constexpr char kVerifiableConversionEnvelopeNonceKey[] = "nonce";

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_USER_DATA_FIXED_CONVERSION_USER_DATA_CONSTANTS_H_
