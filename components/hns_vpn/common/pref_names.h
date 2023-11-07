/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_COMMON_PREF_NAMES_H_
#define HNS_COMPONENTS_HNS_VPN_COMMON_PREF_NAMES_H_

#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "build/build_config.h"

namespace hns_vpn {
namespace prefs {
constexpr char kManagedHnsVPNDisabled[] =
    "hns.hns_vpn.disabled_by_policy";
constexpr char kHnsVPNLocalStateMigrated[] = "hns.hns_vpn.migrated";
constexpr char kHnsVPNRootPref[] = "hns.hns_vpn";
constexpr char kHnsVPNShowButton[] = "hns.hns_vpn.show_button";
constexpr char kHnsVPNRegionList[] = "hns.hns_vpn.region_list";
// Cached fetched date for trying to refresh region_list once per day
constexpr char kHnsVPNRegionListFetchedDate[] =
    "hns.hns_vpn.region_list_fetched_date";
constexpr char kHnsVPNDeviceRegion[] = "hns.hns_vpn.device_region_name";
constexpr char kHnsVPNSelectedRegion[] =
    "hns.hns_vpn.selected_region_name";
#if BUILDFLAG(IS_WIN)
constexpr char kHnsVpnShowDNSPolicyWarningDialog[] =
    "hns.hns_vpn.show_dns_policy_warning_dialog";
constexpr char kHnsVPNShowNotificationDialog[] =
    "hns.hns_vpn.show_notification_dialog";
constexpr char kHnsVPNWireguardFallbackDialog[] =
    "hns.hns_vpn.show_wireguard_fallback_dialog";
#endif  // BUILDFLAG(IS_WIN)
#if BUILDFLAG(ENABLE_HNS_VPN_WIREGUARD)
constexpr char kHnsVPNWireguardEnabled[] =
    "hns.hns_vpn.wireguard_enabled";
#endif
constexpr char kHnsVPNWireguardProfileCredentials[] =
    "hns.hns_vpn.wireguard.profile_credentials";
constexpr char kHnsVPNEnvironment[] = "hns.hns_vpn.env";
// Dict that has subscriber credential its expiration date.
constexpr char kHnsVPNSubscriberCredential[] =
    "hns.hns_vpn.subscriber_credential";

// Time that session expired occurs.
constexpr char kHnsVPNSessionExpiredDate[] =
    "hns.hns_vpn.session_expired_date";

#if BUILDFLAG(IS_ANDROID)
extern const char kHnsVPNPurchaseTokenAndroid[];
extern const char kHnsVPNPackageAndroid[];
extern const char kHnsVPNProductIdAndroid[];
#endif

constexpr char kHnsVPNFirstUseTime[] = "hns.hns_vpn.first_use_time";
constexpr char kHnsVPNLastUseTime[] = "hns.hns_vpn.last_use_time";
constexpr char kHnsVPNUsedSecondDay[] = "hns.hns_vpn.used_second_day";
constexpr char kHnsVPNDaysInMonthUsed[] =
    "hns.hns_vpn.days_in_month_used";
}  // namespace prefs

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_COMMON_PREF_NAMES_H_
