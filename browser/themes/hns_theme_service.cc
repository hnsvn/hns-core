/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/themes/hns_theme_service.h"

#include <memory>

#include "hns/browser/extensions/hns_theme_event_router.h"

HnsThemeService::HnsThemeService(Profile* profile,
                                     const ThemeHelper& theme_helper)
    : ThemeService(profile, theme_helper) {
  hns_theme_event_router_ =
      std::make_unique<extensions::HnsThemeEventRouter>(profile);
}

HnsThemeService::~HnsThemeService() = default;

void HnsThemeService::SetHnsThemeEventRouterForTesting(
    extensions::HnsThemeEventRouter* mock_router) {
  hns_theme_event_router_.reset(mock_router);
}
