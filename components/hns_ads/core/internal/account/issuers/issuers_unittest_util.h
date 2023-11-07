/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_ISSUERS_UNITTEST_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_ISSUERS_UNITTEST_UTIL_H_

#include <string>

#include "hns/components/hns_ads/core/internal/account/issuers/public_key_alias.h"

namespace hns_ads {

struct IssuersInfo;

std::string BuildIssuersUrlResponseBodyForTesting();

IssuersInfo BuildIssuersForTesting(
    int ping,
    const PublicKeyMap& confirmations_public_keys,
    const PublicKeyMap& payments_public_keys);
IssuersInfo BuildIssuersForTesting();
void BuildAndSetIssuersForTesting();

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_ISSUERS_UNITTEST_UTIL_H_
