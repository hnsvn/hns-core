/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/webui/settings/site_settings_helper.h"

#include "base/containers/cxx20_erase_vector.h"
#include "hns/components/hns_shields/common/hns_shield_constants.h"
#include "third_party/blink/public/common/features.h"

#define HasRegisteredGroupName HasRegisteredGroupName_ChromiumImpl
#define ContentSettingsTypeToGroupName \
  ContentSettingsTypeToGroupName_ChromiumImpl
#define GetVisiblePermissionCategories \
  GetVisiblePermissionCategories_ChromiumImpl

// clang-format off
#define HNS_CONTENT_SETTINGS_TYPE_GROUP_NAMES_LIST               \
  {ContentSettingsType::HNS_ADS, nullptr},                       \
  {ContentSettingsType::HNS_COSMETIC_FILTERING, nullptr},        \
  {ContentSettingsType::HNS_TRACKERS, nullptr},                  \
  {ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES, nullptr}, \
  {ContentSettingsType::HNS_FINGERPRINTING_V2, nullptr},         \
  {ContentSettingsType::HNS_SHIELDS, nullptr},                   \
  {ContentSettingsType::HNS_REFERRERS, nullptr},                 \
  {ContentSettingsType::HNS_COOKIES, nullptr},                   \
  {ContentSettingsType::HNS_SPEEDREADER, nullptr},               \
  {ContentSettingsType::HNS_ETHEREUM, nullptr},                  \
  {ContentSettingsType::HNS_SOLANA, nullptr},                    \
  {ContentSettingsType::HNS_GOOGLE_SIGN_IN, nullptr},            \
  {ContentSettingsType::HNS_HTTPS_UPGRADE, nullptr},             \
  {ContentSettingsType::HNS_REMEMBER_1P_STORAGE, nullptr},       \
  {ContentSettingsType::HNS_LOCALHOST_ACCESS, nullptr},
// clang-format on

#define HNS_SITE_SETTINGS_HELPER_CONTENT_SETTINGS_TYPE_FROM_GROUP_NAME \
  if (name == "autoplay")                                                \
    return ContentSettingsType::AUTOPLAY;                                \
  if (name == "googleSignIn")                                            \
    return ContentSettingsType::HNS_GOOGLE_SIGN_IN;                    \
  if (name == "localhostAccess")                                         \
    return ContentSettingsType::HNS_LOCALHOST_ACCESS;                  \
  if (name == "ethereum")                                                \
    return ContentSettingsType::HNS_ETHEREUM;                          \
  if (name == "solana")                                                  \
    return ContentSettingsType::HNS_SOLANA;                            \
  if (name == hns_shields::kHnsShields)                              \
    return ContentSettingsType::HNS_SHIELDS;

#include "src/chrome/browser/ui/webui/settings/site_settings_helper.cc"

#undef HNS_CONTENT_SETTINGS_TYPE_GROUP_NAMES_LIST
#undef HNS_SITE_SETTINGS_HELPER_CONTENT_SETTINGS_TYPE_FROM_GROUP_NAME
#undef GetVisiblePermissionCategories
#undef ContentSettingsTypeToGroupName
#undef HasRegisteredGroupName

namespace site_settings {

bool HasRegisteredGroupName(ContentSettingsType type) {
  if (type == ContentSettingsType::AUTOPLAY)
    return true;
  if (type == ContentSettingsType::HNS_GOOGLE_SIGN_IN)
    return true;
  if (type == ContentSettingsType::HNS_LOCALHOST_ACCESS) {
    return true;
  }
  if (type == ContentSettingsType::HNS_ETHEREUM)
    return true;
  if (type == ContentSettingsType::HNS_SOLANA)
    return true;
  if (type == ContentSettingsType::HNS_SHIELDS)
    return true;
  return HasRegisteredGroupName_ChromiumImpl(type);
}

base::StringPiece ContentSettingsTypeToGroupName(ContentSettingsType type) {
  if (type == ContentSettingsType::AUTOPLAY)
    return "autoplay";
  if (type == ContentSettingsType::HNS_GOOGLE_SIGN_IN)
    return "googleSignIn";
  if (type == ContentSettingsType::HNS_LOCALHOST_ACCESS) {
    return "localhostAccess";
  }
  if (type == ContentSettingsType::HNS_ETHEREUM)
    return "ethereum";
  if (type == ContentSettingsType::HNS_SOLANA)
    return "solana";
  if (type == ContentSettingsType::HNS_SHIELDS)
    return hns_shields::kHnsShields;
  return ContentSettingsTypeToGroupName_ChromiumImpl(type);
}

const std::vector<ContentSettingsType>& GetVisiblePermissionCategories() {
  static base::NoDestructor<std::vector<ContentSettingsType>> types{
      {ContentSettingsType::AUTOPLAY, ContentSettingsType::HNS_ETHEREUM,
       ContentSettingsType::HNS_SOLANA,
       ContentSettingsType::HNS_GOOGLE_SIGN_IN,
       ContentSettingsType::HNS_LOCALHOST_ACCESS}};
  static bool initialized = false;
  if (!initialized) {
    auto& base_types = GetVisiblePermissionCategories_ChromiumImpl();
    types->insert(types->end(), base_types.begin(), base_types.end());

    if (!base::FeatureList::IsEnabled(blink::features::kHnsWebSerialAPI)) {
      base::Erase(*types, ContentSettingsType::SERIAL_GUARD);
    }

    initialized = true;
  }

  return *types;
}

}  // namespace site_settings
