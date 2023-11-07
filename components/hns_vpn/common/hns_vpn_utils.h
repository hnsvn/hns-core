/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_COMMON_HNS_VPN_UTILS_H_
#define HNS_COMPONENTS_HNS_VPN_COMMON_HNS_VPN_UTILS_H_

#include <string>

#include "build/build_config.h"

class PrefRegistrySimple;
class PrefService;
namespace user_prefs {
class PrefRegistrySyncable;
}  // namespace user_prefs

namespace version_info {
enum class Channel;
}  // namespace version_info

namespace hns_vpn {

std::string GetHnsVPNEntryName(version_info::Channel channel);
bool IsHnsVPNEnabled(PrefService* prefs);
bool IsHnsVPNFeatureEnabled();
bool IsHnsVPNDisabledByPolicy(PrefService* prefs);
std::string GetHnsVPNPaymentsEnv(const std::string& env);
std::string GetManageUrl(const std::string& env);
void MigrateVPNSettings(PrefService* profile_prefs, PrefService* local_prefs);
void RegisterLocalStatePrefs(PrefRegistrySimple* registry);
void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);
void RegisterAndroidProfilePrefs(PrefRegistrySimple* registry);
bool HasValidSubscriberCredential(PrefService* local_prefs);
std::string GetSubscriberCredential(PrefService* local_prefs);
bool HasValidSkusCredential(PrefService* local_prefs);
std::string GetSkusCredential(PrefService* local_prefs);
bool IsHnsVPNWireguardEnabled(PrefService* local_state);
#if BUILDFLAG(IS_WIN)
void MigrateWireguardFeatureFlag(PrefService* local_prefs);
#endif  // BUILDFLAG(IS_WIN)
}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_COMMON_HNS_VPN_UTILS_H_
