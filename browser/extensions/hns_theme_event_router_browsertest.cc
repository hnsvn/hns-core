/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/hns_theme_event_router.h"
#include "hns/browser/themes/hns_dark_mode_utils.h"
#include "hns/browser/themes/hns_theme_service.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "testing/gmock/include/gmock/gmock.h"

using HnsThemeEventRouterBrowserTest = InProcessBrowserTest;

namespace extensions {

class MockHnsThemeEventRouter : public HnsThemeEventRouter {
 public:
  using HnsThemeEventRouter::HnsThemeEventRouter;
  ~MockHnsThemeEventRouter() override = default;

  MOCK_METHOD0(Notify, void());
};

}  // namespace extensions

IN_PROC_BROWSER_TEST_F(HnsThemeEventRouterBrowserTest,
                       ThemeChangeTest) {
  dark_mode::SetHnsDarkModeType(
      dark_mode::HnsDarkModeType::HNS_DARK_MODE_TYPE_DARK);

  extensions::MockHnsThemeEventRouter* mock_router =
      new extensions::MockHnsThemeEventRouter(browser()->profile());
  HnsThemeService* service = static_cast<HnsThemeService*>(
      ThemeServiceFactory::GetForProfile(browser()->profile()));
  service->SetHnsThemeEventRouterForTesting(mock_router);

  EXPECT_CALL(*mock_router, Notify()).Times(1);
  dark_mode::SetHnsDarkModeType(
      dark_mode::HnsDarkModeType::HNS_DARK_MODE_TYPE_LIGHT);

  EXPECT_CALL(*mock_router, Notify()).Times(1);
  dark_mode::SetHnsDarkModeType(
      dark_mode::HnsDarkModeType::HNS_DARK_MODE_TYPE_DARK);

  EXPECT_CALL(*mock_router, Notify()).Times(0);
  dark_mode::SetHnsDarkModeType(
      dark_mode::HnsDarkModeType::HNS_DARK_MODE_TYPE_DARK);
}
