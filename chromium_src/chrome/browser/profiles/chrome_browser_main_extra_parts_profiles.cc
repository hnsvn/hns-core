/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define AddProfilesExtraParts AddProfilesExtraParts_ChromiumImpl
#include "src/chrome/browser/profiles/chrome_browser_main_extra_parts_profiles.cc"
#undef AddProfilesExtraParts

#include "hns/browser/browser_context_keyed_service_factories.h"

namespace {

class HnsBrowserMainExtraPartsProfiles
    : public ChromeBrowserMainExtraPartsProfiles {
 public:
  HnsBrowserMainExtraPartsProfiles()
      : ChromeBrowserMainExtraPartsProfiles() {}

  HnsBrowserMainExtraPartsProfiles(
      const HnsBrowserMainExtraPartsProfiles&) = delete;
  HnsBrowserMainExtraPartsProfiles& operator=(
      const HnsBrowserMainExtraPartsProfiles&) = delete;

  void PreProfileInit() override {
    hns::EnsureBrowserContextKeyedServiceFactoriesBuilt();
    ChromeBrowserMainExtraPartsProfiles::PreProfileInit();
  }
};

}  // namespace

namespace chrome {

void AddProfilesExtraParts(ChromeBrowserMainParts* main_parts) {
  main_parts->AddParts(std::make_unique<HnsBrowserMainExtraPartsProfiles>());
}

}  // namespace chrome
