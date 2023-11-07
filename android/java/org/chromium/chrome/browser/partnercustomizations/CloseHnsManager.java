/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.partnercustomizations;

import android.content.SharedPreferences;

import org.chromium.base.ContextUtils;

public class CloseHnsManager {
    private static final String CLOSING_ALL_TABS_CLOSES_HNS = "closing_all_tabs_closes_hns";

    public static boolean shouldCloseAppWithZeroTabs() {
        return getClosingAllTabsClosesHnsEnabled();
    }

    public static boolean getClosingAllTabsClosesHnsEnabled() {
        return ContextUtils.getAppSharedPreferences().getBoolean(CLOSING_ALL_TABS_CLOSES_HNS, false);
    }

    public static void setClosingAllTabsClosesHnsEnabled(boolean enable) {
        SharedPreferences.Editor sharedPreferencesEditor =
            ContextUtils.getAppSharedPreferences().edit();
        sharedPreferencesEditor.putBoolean(CLOSING_ALL_TABS_CLOSES_HNS, enable);
        sharedPreferencesEditor.apply();
    }
}
