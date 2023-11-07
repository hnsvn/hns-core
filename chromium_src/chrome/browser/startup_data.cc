/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/browser_context_keyed_service_factories.h"
#include "chrome/browser/profiles/chrome_browser_main_extra_parts_profiles.h"

namespace {

class HnsBrowserMainExtraPartsProfiles
    : public ChromeBrowserMainExtraPartsProfiles {
 public:
  HnsBrowserMainExtraPartsProfiles() = default;
  HnsBrowserMainExtraPartsProfiles(
      const HnsBrowserMainExtraPartsProfiles&) = delete;
  HnsBrowserMainExtraPartsProfiles& operator=(
      const HnsBrowserMainExtraPartsProfiles&) = delete;
  ~HnsBrowserMainExtraPartsProfiles() override = default;

  static void EnsureBrowserContextKeyedServiceFactoriesBuilt() {
    ChromeBrowserMainExtraPartsProfiles::
        EnsureBrowserContextKeyedServiceFactoriesBuilt();
    hns::EnsureBrowserContextKeyedServiceFactoriesBuilt();
  }
};

}  // namespace

#define ChromeBrowserMainExtraPartsProfiles HnsBrowserMainExtraPartsProfiles
#include "src/chrome/browser/startup_data.cc"
#undef ChromeBrowserMainExtraPartsProfiles
