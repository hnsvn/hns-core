/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/common/hns_vpn_utils.h"

#include <utility>

#include "base/feature_list.h"
#include "base/json/json_writer.h"
#include "base/json/values_util.h"
#include "base/notreached.h"
#include "base/strings/string_split.h"
#include "hns/components/hns_vpn/common/hns_vpn_constants.h"
#include "hns/components/hns_vpn/common/features.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#include "hns/components/p3a_utils/feature_usage.h"
#include "hns/components/skus/browser/skus_utils.h"
#include "hns/components/skus/common/features.h"
#include "build/build_config.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "components/version_info/channel.h"

#if BUILDFLAG(IS_WIN)
#include "hns/components/hns_vpn/common/wireguard/win/wireguard_utils_win.h"
#endif

namespace hns_vpn {

namespace {
void RegisterVPNLocalStatePrefs(PrefRegistrySimple* registry) {
#if !BUILDFLAG(IS_ANDROID)
  registry->RegisterListPref(prefs::kHnsVPNRegionList);
  registry->RegisterTimePref(prefs::kHnsVPNRegionListFetchedDate, {});
  registry->RegisterStringPref(prefs::kHnsVPNDeviceRegion, "");
  registry->RegisterStringPref(prefs::kHnsVPNSelectedRegion, "");
#endif
  registry->RegisterStringPref(prefs::kHnsVPNEnvironment,
                               skus::GetDefaultEnvironment());
  registry->RegisterStringPref(prefs::kHnsVPNWireguardProfileCredentials, "");
  registry->RegisterDictionaryPref(prefs::kHnsVPNRootPref);
  registry->RegisterDictionaryPref(prefs::kHnsVPNSubscriberCredential);
  registry->RegisterBooleanPref(prefs::kHnsVPNLocalStateMigrated, false);
  registry->RegisterTimePref(prefs::kHnsVPNSessionExpiredDate, {});
#if BUILDFLAG(ENABLE_HNS_VPN_WIREGUARD)
  registry->RegisterBooleanPref(prefs::kHnsVPNWireguardEnabled, false);
#endif
}

}  // namespace

bool IsHnsVPNWireguardEnabled(PrefService* local_state) {
  DCHECK(IsHnsVPNFeatureEnabled());
#if BUILDFLAG(ENABLE_HNS_VPN_WIREGUARD)
  return local_state->GetBoolean(prefs::kHnsVPNWireguardEnabled);
#else
  return false;
#endif
}
#if BUILDFLAG(IS_WIN)
void MigrateWireguardFeatureFlag(PrefService* local_prefs) {
  auto* wireguard_enabled_pref =
      local_prefs->FindPreference(prefs::kHnsVPNWireguardEnabled);
  if (wireguard_enabled_pref && wireguard_enabled_pref->IsDefaultValue()) {
    local_prefs->SetBoolean(
        prefs::kHnsVPNWireguardEnabled,
        base::FeatureList::IsEnabled(features::kHnsVPNUseWireguardService) &&
            hns_vpn::wireguard::IsWireguardServiceRegistered());
  }
}
#endif  // BUILDFLAG(IS_WIN)
void MigrateVPNSettings(PrefService* profile_prefs, PrefService* local_prefs) {
  if (local_prefs->GetBoolean(prefs::kHnsVPNLocalStateMigrated)) {
    return;
  }

  if (!profile_prefs->HasPrefPath(prefs::kHnsVPNRootPref)) {
    local_prefs->SetBoolean(prefs::kHnsVPNLocalStateMigrated, true);
    return;
  }
  base::Value::Dict obsolete_pref =
      profile_prefs->GetDict(prefs::kHnsVPNRootPref).Clone();
  base::Value::Dict result;
  if (local_prefs->HasPrefPath(prefs::kHnsVPNRootPref)) {
    result = local_prefs->GetDict(prefs::kHnsVPNRootPref).Clone();
    auto& result_dict = result;
    result_dict.Merge(std::move(obsolete_pref));
  } else {
    result = std::move(obsolete_pref);
  }
  // Do not migrate hns_vpn::prefs::kHnsVPNShowButton, we want it to be
  // inside the profile preferences.
  auto tokens =
      base::SplitString(hns_vpn::prefs::kHnsVPNShowButton, ".",
                        base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY);
  if (result.FindBool(tokens.back())) {
    result.Remove(tokens.back());
  }
  local_prefs->Set(prefs::kHnsVPNRootPref, base::Value(std::move(result)));
  local_prefs->SetBoolean(prefs::kHnsVPNLocalStateMigrated, true);

  bool show_button =
      profile_prefs->GetBoolean(hns_vpn::prefs::kHnsVPNShowButton);
  profile_prefs->ClearPref(prefs::kHnsVPNRootPref);
  // Set kHnsVPNShowButton back, it is only one per profile preference for
  // now.
  profile_prefs->SetBoolean(hns_vpn::prefs::kHnsVPNShowButton, show_button);
}

bool IsHnsVPNDisabledByPolicy(PrefService* prefs) {
  DCHECK(prefs);
  return prefs->FindPreference(prefs::kManagedHnsVPNDisabled) &&
  // Need to investigate more about this.
  // IsManagedPreference() gives false on macOS when it's configured by
  // "defaults write com.hns.Browser.beta HnsVPNDisabled -bool true".
  // As kManagedHnsVPNDisabled is false by default and only can be set
  // by policy, I think skipping this condition checking will be fine.
#if !BUILDFLAG(IS_MAC)
         prefs->IsManagedPreference(prefs::kManagedHnsVPNDisabled) &&
#endif
         prefs->GetBoolean(prefs::kManagedHnsVPNDisabled);
}

bool IsHnsVPNFeatureEnabled() {
  return base::FeatureList::IsEnabled(hns_vpn::features::kHnsVPN) &&
         base::FeatureList::IsEnabled(skus::features::kSkusFeature);
}

bool IsHnsVPNEnabled(PrefService* prefs) {
  return !IsHnsVPNDisabledByPolicy(prefs) && IsHnsVPNFeatureEnabled();
}

std::string GetHnsVPNEntryName(version_info::Channel channel) {
  constexpr char kHnsVPNEntryName[] = "HnsVPN";

  const std::string entry_name(kHnsVPNEntryName);
  switch (channel) {
    case version_info::Channel::UNKNOWN:
      return entry_name + "Development";
    case version_info::Channel::CANARY:
      return entry_name + "Nightly";
    case version_info::Channel::DEV:
      return entry_name + "Dev";
    case version_info::Channel::BETA:
      return entry_name + "Beta";
    case version_info::Channel::STABLE:
      return entry_name;
    default:
      return entry_name;
  }
}

std::string GetManageUrl(const std::string& env) {
  if (env == skus::kEnvProduction)
    return hns_vpn::kManageUrlProd;
  if (env == skus::kEnvStaging)
    return hns_vpn::kManageUrlStaging;
  if (env == skus::kEnvDevelopment)
    return hns_vpn::kManageUrlDev;

  NOTREACHED();
  return hns_vpn::kManageUrlProd;
}

// On desktop, the environment is tied to SKUs because you would purchase it
// from `account.hns.com` (or similar, based on env). The credentials for VPN
// will always be in the same environment as the SKU environment.
//
// When the vendor receives a credential from us during auth, it also includes
// the environment. The vendor then can do a lookup using Payment Service.
std::string GetHnsVPNPaymentsEnv(const std::string& env) {
  // Use same string as payment env.
  return env;
}

void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry) {
  registry->RegisterBooleanPref(prefs::kManagedHnsVPNDisabled, false);
  registry->RegisterDictionaryPref(prefs::kHnsVPNRootPref);
  registry->RegisterBooleanPref(prefs::kHnsVPNShowButton, true);
#if BUILDFLAG(IS_WIN)
  registry->RegisterBooleanPref(prefs::kHnsVPNShowNotificationDialog, true);
  registry->RegisterBooleanPref(prefs::kHnsVPNWireguardFallbackDialog, true);
#endif
#if BUILDFLAG(IS_ANDROID)
  registry->RegisterStringPref(prefs::kHnsVPNPurchaseTokenAndroid, "");
  registry->RegisterStringPref(prefs::kHnsVPNPackageAndroid, "");
  registry->RegisterStringPref(prefs::kHnsVPNProductIdAndroid, "");
#endif
}

void RegisterLocalStatePrefs(PrefRegistrySimple* registry) {
  p3a_utils::RegisterFeatureUsagePrefs(
      registry, prefs::kHnsVPNFirstUseTime, prefs::kHnsVPNLastUseTime,
      prefs::kHnsVPNUsedSecondDay, prefs::kHnsVPNDaysInMonthUsed, nullptr);
  RegisterVPNLocalStatePrefs(registry);
}

bool HasValidSubscriberCredential(PrefService* local_prefs) {
  const base::Value::Dict& sub_cred_dict =
      local_prefs->GetDict(prefs::kHnsVPNSubscriberCredential);
  if (sub_cred_dict.empty())
    return false;

  const std::string* cred = sub_cred_dict.FindString(kSubscriberCredentialKey);
  const base::Value* expiration_time_value =
      sub_cred_dict.Find(kSubscriberCredentialExpirationKey);

  if (!cred || !expiration_time_value)
    return false;

  if (cred->empty())
    return false;

  auto expiration_time = base::ValueToTime(expiration_time_value);
  if (!expiration_time || expiration_time < base::Time::Now())
    return false;

  return true;
}

std::string GetSubscriberCredential(PrefService* local_prefs) {
  if (!HasValidSubscriberCredential(local_prefs))
    return "";
  const base::Value::Dict& sub_cred_dict =
      local_prefs->GetDict(prefs::kHnsVPNSubscriberCredential);
  const std::string* cred = sub_cred_dict.FindString(kSubscriberCredentialKey);
  DCHECK(cred);
  return *cred;
}

bool HasValidSkusCredential(PrefService* local_prefs) {
  const base::Value::Dict& sub_cred_dict =
      local_prefs->GetDict(prefs::kHnsVPNSubscriberCredential);
  if (sub_cred_dict.empty()) {
    return false;
  }

  const std::string* skus_cred = sub_cred_dict.FindString(kSkusCredentialKey);
  const base::Value* expiration_time_value =
      sub_cred_dict.Find(kSubscriberCredentialExpirationKey);

  if (!skus_cred || !expiration_time_value) {
    return false;
  }

  if (skus_cred->empty()) {
    return false;
  }

  auto expiration_time = base::ValueToTime(expiration_time_value);
  if (!expiration_time || expiration_time < base::Time::Now()) {
    return false;
  }

  return true;
}

std::string GetSkusCredential(PrefService* local_prefs) {
  CHECK(HasValidSkusCredential(local_prefs))
      << "Don't call when there is no valid skus credential.";

  const base::Value::Dict& sub_cred_dict =
      local_prefs->GetDict(prefs::kHnsVPNSubscriberCredential);
  const std::string* skus_cred = sub_cred_dict.FindString(kSkusCredentialKey);
  DCHECK(skus_cred);
  return *skus_cred;
}

}  // namespace hns_vpn
