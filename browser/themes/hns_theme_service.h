/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_THEMES_HNS_THEME_SERVICE_H_
#define HNS_BROWSER_THEMES_HNS_THEME_SERVICE_H_

#include <memory>

#include "base/gtest_prod_util.h"
#include "chrome/browser/themes/theme_service.h"

namespace extensions {
class HnsThemeEventRouter;
}  // namespace extensions

class Profile;

class HnsThemeService : public ThemeService {
 public:
  explicit HnsThemeService(Profile* profile, const ThemeHelper& theme_helper);
  ~HnsThemeService() override;

 private:
  FRIEND_TEST_ALL_PREFIXES(HnsThemeEventRouterBrowserTest, ThemeChangeTest);

  // Own |mock_router|.
  void SetHnsThemeEventRouterForTesting(
      extensions::HnsThemeEventRouter* mock_router);

  std::unique_ptr<extensions::HnsThemeEventRouter> hns_theme_event_router_;
};

#endif  // HNS_BROWSER_THEMES_HNS_THEME_SERVICE_H_
