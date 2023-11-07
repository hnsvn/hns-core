/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SHIELDS_BROWSER_HNS_SHIELDS_P3A_H_
#define HNS_COMPONENTS_HNS_SHIELDS_BROWSER_HNS_SHIELDS_P3A_H_

#include "hns/components/hns_shields/browser/hns_shields_util.h"

class PrefRegistrySimple;
class PrefService;
class HostContentSettingsMap;

namespace hns_shields {

constexpr char kUsagePrefName[] = "hns_shields.p3a_usage";
constexpr char kFirstReportedPrefName[] = "hns_shields.p3a_first_reported_v2";

constexpr char kAdsStrictCountPrefName[] =
    "hns_shields.p3a_ads_strict_domain_count";
constexpr char kAdsStandardCountPrefName[] =
    "hns_shields.p3a_ads_standard_domain_count";
constexpr char kAdsAllowCountPrefName[] =
    "hns_shields.p3a_ads_allow_domain_count";
constexpr char kFPStrictCountPrefName[] =
    "hns_shields.p3a_fp_strict_domain_count";
constexpr char kFPStandardCountPrefName[] =
    "hns_shields.p3a_fp_standard_domain_count";
constexpr char kFPAllowCountPrefName[] =
    "hns_shields.p3a_fp_allow_domain_count";

constexpr char kAdsSettingHistogramName[] = "Hns.Shields.AdBlockSetting";
constexpr char kFingerprintSettingHistogramName[] =
    "Hns.Shields.FingerprintBlockSetting";
constexpr char kUsageStatusHistogramName[] = "Hns.Shields.UsageStatus";
constexpr char kDomainAdsSettingsAboveHistogramName[] =
    "Hns.Shields.DomainAdsSettingsAboveGlobal";
constexpr char kDomainAdsSettingsBelowHistogramName[] =
    "Hns.Shields.DomainAdsSettingsBelowGlobal";
constexpr char kDomainFPSettingsAboveHistogramName[] =
    "Hns.Shields.DomainFingerprintSettingsAboveGlobal";
constexpr char kDomainFPSettingsBelowHistogramName[] =
    "Hns.Shields.DomainFingerprintSettingsBelowGlobal";
// Note: append-only enumeration! Never remove any existing values, as this enum
// is used to bucket a UMA histogram, and removing values breaks that.
enum ShieldsIconUsage {
  kNeverClicked,
  kClicked,
  kShutOffShields,
  kChangedPerSiteShields,
  kSize,
};

// We save latest value to local state and compare new values with it.
// The idea is to write to a histogram only the highest value (e.g. we are
// not interested in |kClicked| event if the user already turned off shields.
// Sine P3A sends only latest written values, these is enough for our current
// goals.
void MaybeRecordShieldsUsageP3A(ShieldsIconUsage usage,
                                PrefService* local_state);

// Records to global ads setting histogram: Hns.Shields.AdBlockSetting
void RecordShieldsAdsSetting(ControlType setting);

// Records to global FP setting histogram: Hns.Shields.FingerprintBlockSetting
void RecordShieldsFingerprintSetting(ControlType setting);

// To be called when the global setting changes.
// Will update domain setting count histograms.
void RecordShieldsDomainSettingCounts(PrefService* profile_prefs,
                                      bool is_fingerprint,
                                      ControlType global_setting);

// To be called when a domain setting changes.
// Will update internal pref counts and update domain setting count histograms.
void RecordShieldsDomainSettingCountsWithChange(PrefService* profile_prefs,
                                                bool is_fingerprint,
                                                ControlType global_setting,
                                                ControlType* prev_setting,
                                                ControlType new_setting);

void RegisterShieldsP3ALocalPrefs(PrefRegistrySimple* local_state);

void RegisterShieldsP3AProfilePrefs(PrefRegistrySimple* local_state);

// To be called at initialization. Will count all domain settings and
// record to all histograms, if executed for the first time.
void MaybeRecordInitialShieldsSettings(PrefService* profile_prefs,
                                       HostContentSettingsMap* map);

}  // namespace hns_shields

#endif  // HNS_COMPONENTS_HNS_SHIELDS_BROWSER_HNS_SHIELDS_P3A_H_
