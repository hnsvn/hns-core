/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import android.content.Intent;
import android.content.SharedPreferences;

import org.chromium.base.HnsPreferenceKeys;
import org.chromium.base.ContextUtils;

// see org.hns.bytecode.HnsLaunchIntentDispatcherClassAdapter
public class HnsLaunchIntentDispatcher {
    public static boolean isCustomTabIntent(Intent intent) {
        if (!useCustomTabs()) {
            return false;
        }
        return LaunchIntentDispatcher.isCustomTabIntent(intent);
    }

    public static boolean useCustomTabs() {
        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        return sharedPreferences.getBoolean(HnsPreferenceKeys.HNS_USE_CUSTOM_TABS, true);
    }
}
