/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.query_tiles;

import android.content.Context;

import org.chromium.base.Log;
import org.chromium.chrome.browser.app.HnsActivity;
import org.chromium.chrome.browser.preferences.HnsPref;
import org.chromium.chrome.browser.profiles.ProfileManager;
import org.chromium.components.user_prefs.UserPrefs;

public class HnsQueryTileSection {
    private static final String TAG = "HnsQTSection";

    public static int getMaxRowsForMostVisitedTiles(Context context) {
        try {
            if (!ProfileManager.isInitialized()
                    || !UserPrefs.get(HnsActivity.getHnsActivity().getCurrentProfile())
                                .getBoolean(HnsPref.NEW_TAB_PAGE_SHOW_BACKGROUND_IMAGE)) {
                return 2;
            } else {
                return 1;
            }
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "getMaxRowsForMostVisitedTiles ", e);
        }

        return 2;
    }
}
