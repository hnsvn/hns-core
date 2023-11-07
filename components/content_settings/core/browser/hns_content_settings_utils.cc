/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/content_settings/core/browser/hns_content_settings_utils.h"

#include <algorithm>

#include "base/containers/contains.h"
#include "base/no_destructor.h"
#include "base/notreached.h"
#include "base/strings/string_number_conversions.h"
#include "base/values.h"
#include "hns/components/hns_shields/common/hns_shield_constants.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/gurl.h"

namespace {

bool CanPatternBeConvertedToWildcardSchemeAndPort(
    const ContentSettingsPattern& pattern) {
  // 1. Wildcard is already in the desired state.
  // 2. Our firstParty placeholder shouldn't be converted.
  // 3. Patterns that have file:// scheme.
  // 4. We only want to convert patterns that have a specific host, so something
  // like "http://*:80/*" should be left alone.
  if (pattern == ContentSettingsPattern::Wildcard() ||
      pattern == ContentSettingsPattern::FromString("https://firstParty/*") ||
      pattern.GetScheme() == ContentSettingsPattern::SCHEME_FILE ||
      pattern.MatchesAllHosts() || pattern.GetHost().empty())
    return false;
  // Check for the case when the scheme is wildcard, but the port isn't.
  if (pattern.GetScheme() == ContentSettingsPattern::SCHEME_WILDCARD) {
    GURL check_for_port_url("http://" + pattern.ToString());
    return check_for_port_url.has_port();
  }
  GURL url(pattern.ToString());
  if (!url.is_valid() || url.is_empty() || !url.has_host())
    return false;
  if (url.has_scheme())
    return !ContentSettingsPattern::IsNonWildcardDomainNonPortScheme(
        url.scheme_piece());
  return url.has_port();
}

}  // namespace

namespace content_settings {

const std::vector<ContentSettingsType>& GetShieldsContentSettingsTypes() {
  static const base::NoDestructor<std::vector<ContentSettingsType>>
      kShieldsContentSettingsTypes({
          ContentSettingsType::HNS_ADS,
          ContentSettingsType::HNS_COSMETIC_FILTERING,
          ContentSettingsType::HNS_TRACKERS,
          ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES,
          ContentSettingsType::HNS_FINGERPRINTING_V2,
          ContentSettingsType::HNS_SHIELDS,
          ContentSettingsType::HNS_REFERRERS,
          ContentSettingsType::HNS_COOKIES,
      });

  return *kShieldsContentSettingsTypes;
}

std::string GetShieldsContentTypeName(const ContentSettingsType& content_type) {
  switch (content_type) {
    case ContentSettingsType::HNS_ADS:
      return hns_shields::kAds;
    case ContentSettingsType::HNS_COSMETIC_FILTERING:
      return hns_shields::kCosmeticFiltering;
    case ContentSettingsType::HNS_TRACKERS:
      return hns_shields::kTrackers;
    case ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES:
      return hns_shields::kHTTPUpgradableResources;
    case ContentSettingsType::HNS_FINGERPRINTING_V2:
      return hns_shields::kFingerprintingV2;
    case ContentSettingsType::HNS_SHIELDS:
      return hns_shields::kHnsShields;
    case ContentSettingsType::HNS_REFERRERS:
      return hns_shields::kReferrers;
    case ContentSettingsType::HNS_COOKIES:
      return hns_shields::kCookies;
    default:
      NOTREACHED();
      return std::string();
  }
}

bool IsShieldsContentSettingsType(const ContentSettingsType& content_type) {
  return base::Contains(GetShieldsContentSettingsTypes(), content_type);
}

bool IsShieldsContentSettingsTypeName(const std::string& content_type_name) {
  for (auto content_type : GetShieldsContentSettingsTypes()) {
    if (GetShieldsContentTypeName(content_type) == content_type_name)
      return true;
  }
  return false;
}

absl::optional<ContentSettingsPattern> ConvertPatternToWildcardSchemeAndPort(
    const ContentSettingsPattern& pattern) {
  if (!CanPatternBeConvertedToWildcardSchemeAndPort(pattern))
    return absl::nullopt;
  DCHECK(!pattern.GetHost().empty());
  absl::optional<ContentSettingsPattern> new_pattern =
      ContentSettingsPattern::FromString("*://" + pattern.GetHost() + "/*");
  return new_pattern;
}

// Returns the full path in the user preferences store to the Hns Shields
// setting identified by it's name (i.e. |name|).
std::string GetShieldsSettingUserPrefsPath(const std::string& name) {
  return std::string("profile.content_settings.exceptions.").append(name);
}

// Extract a SessionModel from |dict[key]|. Will return
// SessionModel::Durable if no model exists.
content_settings::SessionModel GetSessionModelFromDictionary(
    const base::Value::Dict& dict,
    const char* key) {
  absl::optional<int> model_int = dict.FindInt(key);
  if (!model_int.has_value() ||
      (model_int >
       static_cast<int>(content_settings::SessionModel::kMaxValue)) ||
      (model_int < 0)) {
    model_int = 0;
  }

  content_settings::SessionModel session_model =
      static_cast<content_settings::SessionModel>(model_int.value());
  return session_model;
}

}  // namespace content_settings
