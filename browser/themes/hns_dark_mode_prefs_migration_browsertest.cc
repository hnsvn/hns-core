/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/themes/hns_dark_mode_utils.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"

using DarkModePrefsMigrationTest = InProcessBrowserTest;

IN_PROC_BROWSER_TEST_F(DarkModePrefsMigrationTest, PrefMigrationTest) {
  g_browser_process->local_state()->ClearPref(kHnsDarkMode);
  EXPECT_TRUE(g_browser_process->local_state()->
      FindPreference(kHnsDarkMode)->IsDefaultValue());

  const int dark_mode_type = 2;
  // Set profile prefs explicitly for migration test.
  browser()->profile()->GetPrefs()->SetInteger(kHnsThemeType, dark_mode_type);

  // Migrate and check it's done properly with previous profile prefs value.
  dark_mode::MigrateHnsDarkModePrefs(browser()->profile());
  EXPECT_FALSE(g_browser_process->local_state()->
      FindPreference(kHnsDarkMode)->IsDefaultValue());
  EXPECT_EQ(dark_mode_type,
            g_browser_process->local_state()->GetInteger(kHnsDarkMode));
}
