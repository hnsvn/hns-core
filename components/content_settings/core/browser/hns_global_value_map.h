/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_HNS_GLOBAL_VALUE_MAP_H_
#define HNS_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_HNS_GLOBAL_VALUE_MAP_H_

#include <memory>

#include "components/content_settings/core/browser/content_settings_global_value_map.h"
#include "components/content_settings/core/browser/content_settings_provider.h"
#include "components/content_settings/core/common/content_settings_types.h"

namespace content_settings {

class RuleIterator;

class HnsGlobalValueMap : public GlobalValueMap {
 public:
  HnsGlobalValueMap();
  HnsGlobalValueMap(const HnsGlobalValueMap&) = delete;
  HnsGlobalValueMap& operator=(const HnsGlobalValueMap&) = delete;
  ~HnsGlobalValueMap();

  // Returns nullptr to indicate the RuleIterator is empty.
  std::unique_ptr<RuleIterator> GetRuleIterator(
      ContentSettingsType content_type) const;
  void SetContentSetting(ContentSettingsType content_type,
                         ContentSetting setting);
  ContentSetting GetContentSetting(ContentSettingsType content_type) const;
};

}  // namespace content_settings

#endif  // HNS_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_HNS_GLOBAL_VALUE_MAP_H_
