/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_POLICY_HNS_SIMPLE_POLICY_MAP_H_
#define HNS_BROWSER_POLICY_HNS_SIMPLE_POLICY_MAP_H_

#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "build/build_config.h"
#include "components/policy/core/browser/configuration_policy_handler.h"
#include "components/policy/policy_constants.h"

#if BUILDFLAG(IS_WIN) || BUILDFLAG(IS_MAC) || BUILDFLAG(IS_LINUX)
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/hns_wallet/common/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "hns/components/tor/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/components/ipfs/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/components/hns_vpn/common/pref_names.h"
#endif

namespace policy {

constexpr PolicyToPreferenceMapEntry kHnsSimplePolicyMap[] = {
#if BUILDFLAG(IS_WIN) || BUILDFLAG(IS_MAC) || BUILDFLAG(IS_LINUX)
    {policy::key::kHnsRewardsDisabled,
     hns_rewards::prefs::kDisabledByPolicy, base::Value::Type::BOOLEAN},
    {policy::key::kHnsWalletDisabled, hns_wallet::prefs::kDisabledByPolicy,
     base::Value::Type::BOOLEAN},
    {policy::key::kHnsShieldsDisabledForUrls,
     kManagedHnsShieldsDisabledForUrls, base::Value::Type::LIST},
    {policy::key::kHnsShieldsEnabledForUrls,
     kManagedHnsShieldsEnabledForUrls, base::Value::Type::LIST},
#endif
#if BUILDFLAG(ENABLE_TOR)
    {policy::key::kTorDisabled, tor::prefs::kTorDisabled,
     base::Value::Type::BOOLEAN},
#endif
#if BUILDFLAG(ENABLE_IPFS)
    {policy::key::kIPFSEnabled, kIPFSEnabled, base::Value::Type::BOOLEAN},
#endif
#if BUILDFLAG(ENABLE_HNS_VPN)
    {policy::key::kHnsVPNDisabled, hns_vpn::prefs::kManagedHnsVPNDisabled,
     base::Value::Type::BOOLEAN},
#endif
};

}  // namespace policy

#endif  // HNS_BROWSER_POLICY_HNS_SIMPLE_POLICY_MAP_H_
