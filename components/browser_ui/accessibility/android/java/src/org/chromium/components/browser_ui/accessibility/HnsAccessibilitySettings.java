/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.components.browser_ui.accessibility;

import android.os.Bundle;

import org.chromium.base.HnsFeatureList;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;

public class HnsAccessibilitySettings extends AccessibilitySettings {
    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        super.onCreatePreferences(savedInstanceState, rootKey);

        ChromeSwitchPreference readerForAccessibilityPref =
                (ChromeSwitchPreference) findPreference(PREF_READER_FOR_ACCESSIBILITY);
        if (ChromeFeatureList.isEnabled(HnsFeatureList.HNS_SPEEDREADER)) {
            getPreferenceScreen().removePreference(readerForAccessibilityPref);
        }
    }
}
