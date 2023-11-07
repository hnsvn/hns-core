/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/content_settings/core/browser/content_settings_utils.h"

#define GetRendererContentSettingRules \
  GetRendererContentSettingRules_ChromiumImpl

// Hns's ContentSettingsType::HNS_COSMETIC_FILTERING,
// ContentSettingsType::HNS_SPEEDREADER,  and
// ContentSettingsType::HNS_COOKIES types use
// CONTENT_SETTING_DEFAULT as the initial default value, which is not a valid
// initial default value according to CanTrackLastVisit and
// ParseContentSettingValue: Note that |CONTENT_SETTING_DEFAULT| is encoded as a
// NULL value, so it is not allowed as an integer value. Also, see
// https://github.com/hnsvn/hns-browser/issues/25733
#define HNS_CAN_TRACK_LAST_VISIT                             \
  if (type == ContentSettingsType::HNS_COOKIES ||            \
      type == ContentSettingsType::HNS_COSMETIC_FILTERING || \
      type == ContentSettingsType::HNS_SPEEDREADER) {        \
    return false;                                              \
  }

#include "src/components/content_settings/core/browser/content_settings_utils.cc"
#undef PROTOCOL_HANDLERS
#undef GetRendererContentSettingRules

namespace content_settings {

void GetRendererContentSettingRules(const HostContentSettingsMap* map,
                                    RendererContentSettingRules* rules) {
  GetRendererContentSettingRules_ChromiumImpl(map, rules);
  std::pair<ContentSettingsType, ContentSettingsForOneType*> settings[] = {
      {ContentSettingsType::AUTOPLAY, &rules->autoplay_rules},
      {ContentSettingsType::HNS_FINGERPRINTING_V2,
       &rules->fingerprinting_rules},
      {ContentSettingsType::HNS_SHIELDS, &rules->hns_shields_rules},
      {ContentSettingsType::HNS_COSMETIC_FILTERING,
       &rules->cosmetic_filtering_rules},
  };
  for (const auto& setting : settings) {
    DCHECK(
        RendererContentSettingRules::IsRendererContentSetting(setting.first));
    *setting.second = map->GetSettingsForOneType(setting.first);
  }
}

}  // namespace content_settings
