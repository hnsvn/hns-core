/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.privacy.settings;

import org.chromium.chrome.browser.HnsConfig;
import org.chromium.chrome.browser.preferences.HnsPref;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.components.user_prefs.UserPrefs;

public class HnsPrivacySettingsIPFSUtils {
    public static void setIPFSGatewayPref(boolean preference) {
        if (HnsConfig.IPFS_ENABLED) {
            UserPrefs.get(Profile.getLastUsedRegularProfile())
                    .setInteger(HnsPref.IPFS_RESOLVE_METHOD,
                            preference ? IPFSResolveMethodTypes.IPFS_ASK
                                       : IPFSResolveMethodTypes.IPFS_DISABLED);
        }
    }

    public static boolean getIPFSGatewayPref() {
        if (HnsConfig.IPFS_ENABLED) {
            return UserPrefs.get(Profile.getLastUsedRegularProfile())
                           .getInteger(HnsPref.IPFS_RESOLVE_METHOD)
                    != IPFSResolveMethodTypes.IPFS_DISABLED;
        } else {
            return false;
        }
    }
}
