/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/content_settings/core/browser/hns_global_value_map.h"

#include <memory>

#include "hns/components/content_settings/core/browser/hns_content_settings_utils.h"
#include "components/content_settings/core/browser/content_settings_rule.h"
#include "components/content_settings/core/common/content_settings.h"

namespace content_settings {

HnsGlobalValueMap::HnsGlobalValueMap() = default;

HnsGlobalValueMap::~HnsGlobalValueMap() = default;

std::unique_ptr<RuleIterator> HnsGlobalValueMap::GetRuleIterator(
    ContentSettingsType content_type) const {
  if (content_settings::IsShieldsContentSettingsType(content_type)) {
    return nullptr;
  }
  return GlobalValueMap::GetRuleIterator(content_type);
}

void HnsGlobalValueMap::SetContentSetting(ContentSettingsType content_type,
                                            ContentSetting setting) {
  return GlobalValueMap::SetContentSetting(content_type, setting);
}

ContentSetting HnsGlobalValueMap::GetContentSetting(
    ContentSettingsType content_type) const {
  return GlobalValueMap::GetContentSetting(content_type);
}

}  // namespace content_settings
