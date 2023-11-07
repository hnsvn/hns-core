/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_ISSUERS_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_ISSUERS_UTIL_H_

#include <string>

#include "hns/components/hns_ads/core/internal/account/issuers/issuer_types.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

struct IssuerInfo;
struct IssuersInfo;

void SetIssuers(const IssuersInfo& issuers);
absl::optional<IssuersInfo> GetIssuers();
void ResetIssuers();

bool IsIssuersValid(const IssuersInfo& issuers);

bool HasIssuers();
bool HasIssuersChanged(const IssuersInfo& other);

bool IssuerExistsForType(IssuerType issuer_type);
absl::optional<IssuerInfo> GetIssuerForType(const IssuersInfo& issuers,
                                            IssuerType issuer_type);

bool PublicKeyExistsForIssuerType(IssuerType issuer_type,
                                  const std::string& public_key);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_ISSUERS_UTIL_H_
