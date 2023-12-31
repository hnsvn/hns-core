/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_HNS_VPN_SERVICE_HELPER_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_HNS_VPN_SERVICE_HELPER_H_

#include <memory>
#include <string>
#include <vector>

#include "base/values.h"
#include "hns/components/hns_vpn/common/mojom/hns_vpn.mojom.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

class PrefService;

namespace base {
class Time;
class Value;
}  // namespace base

namespace hns_vpn {
mojom::RegionPtr GetRegionPtrWithNameFromRegionList(
    const std::string& name,
    const std::vector<mojom::Region> region_list);

// False if subscription is expired.
bool IsValidCredentialSummary(const base::Value& summary);
bool IsValidCredentialSummaryButNeedActivation(const base::Value& summary);
bool HasSubscriberCredential(PrefService* local_prefs);
std::string GetSubscriberCredential(PrefService* local_prefs);
absl::optional<base::Time> GetExpirationTime(PrefService* local_prefs);
void SetSubscriberCredential(PrefService* local_prefs,
                             const std::string& subscriber_credential,
                             const base::Time& expiration_time);
void ClearSubscriberCredential(PrefService* local_prefs);
void SetSkusCredential(PrefService* local_prefs,
                       const std::string& skus_credential,
                       const base::Time& expiration_time);
void SetSkusCredentialFetchingRetried(PrefService* local_prefs, bool retried);
bool IsRetriedSkusCredential(PrefService* local_prefs);
base::Time GetExpirationTimeForSkusCredential(PrefService* local_prefs);

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_HNS_VPN_SERVICE_HELPER_H_
