/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/content_settings/core/common/cookie_settings_base.h"

#include "base/auto_reset.h"
#include "base/compiler_specific.h"
#include "base/feature_list.h"
#include "base/no_destructor.h"
#include "base/types/optional_util.h"
#include "components/content_settings/core/common/content_settings.h"
#include "components/content_settings/core/common/content_settings_pattern.h"
#include "components/content_settings/core/common/features.h"
#include "net/base/features.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"
#include "net/cookies/site_for_cookies.h"
#include "third_party/abseil-cpp/absl/base/attributes.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/gurl.h"
#include "url/origin.h"

namespace content_settings {

namespace {

constexpr char kWp[] = "https://[*.]wp.com/*";
constexpr char kWordpress[] = "https://[*.]wordpress.com/*";

// The thread local hns metadata pointer.
ABSL_CONST_INIT thread_local CookieSettingWithHnsMetadata*
    current_cookie_settings_with_hns_metadata = nullptr;

bool HnsIsAllowedThirdParty(const GURL& url,
                              const GURL& first_party_url,
                              const CookieSettingsBase* const cookie_settings) {
  static const base::NoDestructor<
      // url -> first_party_url allow map
      std::vector<std::pair<ContentSettingsPattern, ContentSettingsPattern>>>
      entity_list({{ContentSettingsPattern::FromString(kWp),
                    ContentSettingsPattern::FromString(kWordpress)},
                   {ContentSettingsPattern::FromString(kWordpress),
                    ContentSettingsPattern::FromString(kWp)}});

  if (net::registry_controlled_domains::GetDomainAndRegistry(
          url, net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES) ==
      net::registry_controlled_domains::GetDomainAndRegistry(
          first_party_url,
          net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES))
    return true;

  for (auto i = entity_list->begin(); i != entity_list->end(); ++i) {
    if (i->first.Matches(url) && i->second.Matches(first_party_url))
      return true;
  }

  return false;
}

bool IsFirstPartyAccessAllowed(const GURL& first_party_url,
                               const CookieSettingsBase* const cookie_settings,
                               net::CookieSettingOverrides overrides) {
  ContentSetting setting = cookie_settings->GetCookieSetting(
      first_party_url, first_party_url, overrides, nullptr);
  return cookie_settings->IsAllowed(setting);
}

bool IsSessionOnlyExplicit(
    const CookieSettingWithHnsMetadata& setting_with_hns_metadata) {
  return setting_with_hns_metadata.setting == CONTENT_SETTING_SESSION_ONLY &&
         setting_with_hns_metadata.IsExplicitSetting();
}

}  // namespace

CookieSettingsBase::~CookieSettingsBase() = default;

CookieSettingWithHnsMetadata::CookieSettingWithHnsMetadata() = default;
CookieSettingWithHnsMetadata::CookieSettingWithHnsMetadata(
    const CookieSettingWithHnsMetadata&) = default;
CookieSettingWithHnsMetadata::CookieSettingWithHnsMetadata(
    CookieSettingWithHnsMetadata&&) = default;
CookieSettingWithHnsMetadata& CookieSettingWithHnsMetadata::operator=(
    const CookieSettingWithHnsMetadata&) = default;
CookieSettingWithHnsMetadata& CookieSettingWithHnsMetadata::operator=(
    CookieSettingWithHnsMetadata&&) = default;
CookieSettingWithHnsMetadata::~CookieSettingWithHnsMetadata() = default;

bool CookieSettingWithHnsMetadata::IsExplicitSetting() const {
  return !primary_pattern_matches_all_hosts ||
         !secondary_pattern_matches_all_hosts;
}

bool CookieSettingsBase::ShouldUseEphemeralStorage(
    const GURL& url,
    const net::SiteForCookies& site_for_cookies,
    net::CookieSettingOverrides overrides,
    const absl::optional<url::Origin>& top_frame_origin) const {
  if (!base::FeatureList::IsEnabled(net::features::kHnsEphemeralStorage))
    return false;

  const GURL first_party_url =
      GetFirstPartyURL(site_for_cookies, base::OptionalToPtr(top_frame_origin));

  if (!first_party_url.is_valid())
    return false;

  // Enable ephemeral storage for a first party URL if SESSION_ONLY cookie
  // setting is set and the feature is enabled.
  absl::optional<CookieSettingWithHnsMetadata> first_party_setting;
  if (base::FeatureList::IsEnabled(
          net::features::kHnsFirstPartyEphemeralStorage)) {
    first_party_setting = GetCookieSettingWithHnsMetadata(
        first_party_url, first_party_url, overrides);
    if (IsSessionOnlyExplicit(*first_party_setting)) {
      return true;
    }
  }

  if (net::registry_controlled_domains::SameDomainOrHost(
          first_party_url, url,
          net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES))
    return false;

  bool allow_3p =
      IsCookieAccessAllowedImpl(url, site_for_cookies, top_frame_origin,
                                overrides, /*cookie_settings*/ nullptr);
  bool allow_1p = first_party_setting ? IsAllowed(first_party_setting->setting)
                                      : IsFirstPartyAccessAllowed(
                                            first_party_url, this, overrides);

  // only use ephemeral storage for block 3p
  return allow_1p && !allow_3p;
}

bool CookieSettingsBase::IsEphemeralCookieAccessAllowed(
    const GURL& url,
    const net::SiteForCookies& site_for_cookies,
    const absl::optional<url::Origin>& top_frame_origin,
    net::CookieSettingOverrides overrides) const {
  if (ShouldUseEphemeralStorage(url, site_for_cookies, overrides,
                                top_frame_origin)) {
    return true;
  }

  return IsCookieAccessAllowedImpl(url, site_for_cookies, top_frame_origin,
                                   overrides, /*cookie_settings*/ nullptr);
}

bool CookieSettingsBase::IsFullCookieAccessAllowed(
    const GURL& url,
    const net::SiteForCookies& site_for_cookies,
    const absl::optional<url::Origin>& top_frame_origin,
    net::CookieSettingOverrides overrides,
    CookieSettingWithMetadata* cookie_settings) const {
  return IsCookieAccessAllowedImpl(url, site_for_cookies, top_frame_origin,
                                   overrides, cookie_settings);
}

bool CookieSettingsBase::IsCookieAccessAllowedImpl(
    const GURL& url,
    const net::SiteForCookies& site_for_cookies,
    const absl::optional<url::Origin>& top_frame_origin,
    net::CookieSettingOverrides overrides,
    CookieSettingWithMetadata* cookie_settings) const {
  bool allow = IsChromiumFullCookieAccessAllowed(
      url, site_for_cookies, top_frame_origin, overrides, cookie_settings);

  const bool is_1p_ephemeral_feature_enabled = base::FeatureList::IsEnabled(
      net::features::kHnsFirstPartyEphemeralStorage);
  // If 1PES feature is enabled, we should do additional checks below.
  if (allow && !is_1p_ephemeral_feature_enabled)
    return true;

  const GURL first_party_url =
      GetFirstPartyURL(site_for_cookies, base::OptionalToPtr(top_frame_origin));

  // Determine whether a main frame is ephemeral or Shields are down.
  // This is required to properly handle main and nested frames depending on the
  // main frame mode.
  enum class MainFrameMode {
    // Main frame works as usual, nested 3p frames use ephemeral storage if
    // necessary.
    kDefault,
    // Main frame is in Ephemeral Storage mode, 1p/3p frames use ephemeral
    // storage if necessary.
    kEphemeral,
    // Main frame is in "Shields down" mode, all 1p/3p frames should use
    // persistend storage, *including* 3p frames with enabled "First party
    // ephemeral storage" mode.
    kShieldsDown,
  };
  MainFrameMode main_frame_mode = MainFrameMode::kDefault;
  if (is_1p_ephemeral_feature_enabled) {
    // Get CookieSetting for the main frame and get matched patterns if any.
    CookieSettingWithHnsMetadata setting_with_hns_metadata =
        GetCookieSettingWithHnsMetadata(first_party_url, first_party_url,
                                          overrides);

    // Ephemeral mode for the main frame can be enabled only via explicit rule.
    if (IsSessionOnlyExplicit(setting_with_hns_metadata)) {
      main_frame_mode = MainFrameMode::kEphemeral;
    } else {
      // Disabled shields mode allows everything in nested frames. To properly
      // handle this state we need to know if Shields are down in the main
      // frame. The shields check is done by analyzing the primary and secondary
      // patterns and expecting them to be in a specific state.
      if (setting_with_hns_metadata.setting == CONTENT_SETTING_ALLOW &&
          setting_with_hns_metadata.primary_pattern_matches_all_hosts &&
          !setting_with_hns_metadata.secondary_pattern_matches_all_hosts) {
        main_frame_mode = MainFrameMode::kShieldsDown;
      }
    }
  }

  if (allow) {
    // When the main frame is in ephemeral mode, we should block all non
    // ephemeral-supported activities (service workers, etc.).
    if (main_frame_mode == MainFrameMode::kEphemeral) {
      return false;
    }
    return true;
  }

  if (!IsFirstPartyAccessAllowed(first_party_url, this, overrides)) {
    return false;
  }

  if (HnsIsAllowedThirdParty(url, first_party_url, this))
    return true;

  // This allows Session-only frames to work as usual when Shields are down for
  // the main frame.
  if (is_1p_ephemeral_feature_enabled &&
      main_frame_mode == MainFrameMode::kShieldsDown &&
      IsCookieSessionOnly(url)) {
    return true;
  }

  return false;
}

// Determines whether a 3p cookies block should be applied if a requesting URL
// uses an explicit CONTENT_SETTING_SESSION_ONLY setting.
// By default Chromimum allows all 3p cookies if applied CookieSettingsPatterns
// for the URL are explicit, i.e. if a user added the rule manually then they
// trusts the website. We use explicit setting to enable 1PES mode, but in this
// mode we still want to block 3p frames as usual and not fallback to "allow
// everything" path.
bool CookieSettingsBase::ShouldBlockThirdPartyIfSettingIsExplicit(
    bool block_third_party_cookies,
    ContentSetting cookie_setting,
    bool is_explicit_setting,
    bool is_first_party_allowed_scheme) const {
  return block_third_party_cookies &&
         cookie_setting == CONTENT_SETTING_SESSION_ONLY &&
         is_explicit_setting && !is_first_party_allowed_scheme;
}

CookieSettingWithHnsMetadata
CookieSettingsBase::GetCookieSettingWithHnsMetadata(
    const GURL& url,
    const GURL& first_party_url,
    net::CookieSettingOverrides overrides) const {
  CookieSettingWithHnsMetadata setting_hns_metadata;
  const base::AutoReset<CookieSettingWithHnsMetadata*> resetter(
      &current_cookie_settings_with_hns_metadata, &setting_hns_metadata);
  // GetCookieSetting fills metadata structure implicitly (implemented in
  // GetCookieSettingInternal), the setting value is set explicitly here.
  setting_hns_metadata.setting =
      GetCookieSetting(url, first_party_url, overrides, nullptr);
  return setting_hns_metadata;
}

CookieSettingWithHnsMetadata*
CookieSettingsBase::GetCurrentCookieSettingWithHnsMetadata() {
  // Workaround false-positive MSAN use-of-uninitialized-value on
  // thread_local storage for loaded libraries:
  // https://github.com/google/sanitizers/issues/1265
  MSAN_UNPOISON(&current_cookie_settings_with_hns_metadata,
                sizeof(CookieSettingWithHnsMetadata*));

  return current_cookie_settings_with_hns_metadata;
}

}  // namespace content_settings

// Determines whether a 3p cookies block should be applied if a requesting URL
// uses an explicit 1PES setting (CONTENT_SETTING_SESSION_ONLY). By default
// Chromimum allows all 3p cookies if applied CookieSettingsPatterns for the URL
// were explicit. We use explicit setting to enable 1PES mode, but in this mode
// we still want to block 3p frames as usual and not fallback to "allow
// everything" path.
#define HNS_COOKIE_SETTINGS_BASE_GET_COOKIES_SETTINGS_INTERNAL         \
  if (!block_third && is_third_party_request) {                          \
    block_third = ShouldBlockThirdPartyIfSettingIsExplicit(              \
        ShouldBlockThirdPartyCookies(), setting,                         \
        IsExplicitSetting(setting_info),                                 \
        IsThirdPartyCookiesAllowedScheme(first_party_url.scheme()));     \
  }                                                                      \
  /* Store patterns information to determine if Shields are disabled. */ \
  if (auto* setting_with_hns_metadata =                                \
          GetCurrentCookieSettingWithHnsMetadata()) {                  \
    setting_with_hns_metadata->primary_pattern_matches_all_hosts =     \
        setting_info.primary_pattern.MatchesAllHosts();                  \
    setting_with_hns_metadata->secondary_pattern_matches_all_hosts =   \
        setting_info.secondary_pattern.MatchesAllHosts();                \
  }

#define IsFullCookieAccessAllowed IsChromiumFullCookieAccessAllowed
#include "src/components/content_settings/core/common/cookie_settings_base.cc"
#undef IsFullCookieAccessAllowed
#undef HNS_COOKIE_SETTINGS_BASE_GET_COOKIES_SETTINGS_INTERNAL
