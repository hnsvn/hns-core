/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.tasks.tab_management;

import org.chromium.base.HnsPreferenceKeys;
import org.chromium.chrome.browser.preferences.SharedPreferencesManager;

public class HnsTabUiFeatureUtilities {
    public static boolean isHnsTabGroupsEnabled() {
        return SharedPreferencesManager.getInstance().readBoolean(
                HnsPreferenceKeys.HNS_TAB_GROUPS_ENABLED, true);
    }
}
