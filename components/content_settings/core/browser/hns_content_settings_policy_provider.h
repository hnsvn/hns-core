/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_HNS_CONTENT_SETTINGS_POLICY_PROVIDER_H_
#define HNS_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_HNS_CONTENT_SETTINGS_POLICY_PROVIDER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "components/content_settings/core/browser/content_settings_policy_provider.h"

namespace content_settings {

// With this subclass, shields configuration is persisted across sessions.
class HnsPolicyProvider : public PolicyProvider {
 public:
  explicit HnsPolicyProvider(PrefService* prefs);
  ~HnsPolicyProvider() override;

  HnsPolicyProvider(const HnsPolicyProvider&) = delete;
  HnsPolicyProvider& operator=(const HnsPolicyProvider&) = delete;

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

 private:
  static const PolicyProvider::PrefsForManagedDefaultMapEntry
      kHnsPrefsForManagedDefault[];

  void ReadManagedContentSettings(bool overwrite) override;

  void OnPreferenceChanged(const std::string& name);
  void GetHnsContentSettingsFromPreferences(
      OriginIdentifierValueMap* value_map);
};

}  //  namespace content_settings

#endif  // HNS_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_HNS_CONTENT_SETTINGS_POLICY_PROVIDER_H_
