/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_COMMON_HNS_VPN_CONSTANTS_H_
#define HNS_COMPONENTS_HNS_VPN_COMMON_HNS_VPN_CONSTANTS_H_

#include "build/buildflag.h"

namespace hns_vpn {

constexpr char kManageUrlProd[] = "https://account.hns.com/account/";
constexpr char kManageUrlStaging[] =
    "https://account.hnssoftware.com/account/";
constexpr char kManageUrlDev[] = "https://account.hns.software/account/";

// TODO(simonhong): Update when vpn feedback url is ready.
constexpr char kFeedbackUrl[] = "https://support.hns.com/";
constexpr char kAboutUrl[] = "https://hns.com/firewall-vpn/";

constexpr char kRegionContinentKey[] = "continent";
constexpr char kRegionNameKey[] = "name";
constexpr char kRegionNamePrettyKey[] = "name-pretty";
constexpr char kRegionCountryIsoCodeKey[] = "country-iso-code";
constexpr char kCreateSupportTicket[] = "api/v1.2/partners/support-ticket";
constexpr char kSupportTicketEmailKey[] = "email";
constexpr char kSupportTicketSubjectKey[] = "subject";
constexpr char kSupportTicketSupportTicketKey[] = "support-ticket";
constexpr char kSupportTicketPartnerClientIdKey[] = "partner-client-id";
constexpr char kSupportTicketTimezoneKey[] = "timezone";

constexpr char kVpnHost[] = "connect-api.guardianapp.com";
constexpr char kAllServerRegions[] = "api/v1/servers/all-server-regions";
constexpr char kTimezonesForRegions[] =
    "api/v1.1/servers/timezones-for-regions";
constexpr char kHostnameForRegion[] = "api/v1.2/servers/hostnames-for-region";
constexpr char kProfileCredential[] = "api/v1.1/register-and-create";
constexpr char kCredential[] = "api/v1.3/device/";
constexpr char kVerifyPurchaseToken[] = "api/v1.1/verify-purchase-token";
constexpr char kCreateSubscriberCredentialV12[] =
    "api/v1.2/subscriber-credential/create";
constexpr int kP3AIntervalHours = 24;

constexpr char kSubscriberCredentialKey[] = "credential";
constexpr char kSkusCredentialKey[] = "skus_credential";
constexpr char kRetriedSkusCredentialKey[] = "retried_skus_credential";
constexpr char kSubscriberCredentialExpirationKey[] = "expiration";

#if !BUILDFLAG(IS_ANDROID)
constexpr char kTokenNoLongerValid[] = "Token No Longer Valid";
#endif  // !BUILDFLAG(IS_ANDROID)

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_COMMON_HNS_VPN_CONSTANTS_H_
