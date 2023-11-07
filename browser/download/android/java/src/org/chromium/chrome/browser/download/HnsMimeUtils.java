/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.download;

import org.chromium.base.HnsPreferenceKeys;
import org.chromium.base.ContextUtils;
import org.chromium.base.annotations.CalledByNative;

public class HnsMimeUtils {
    @CalledByNative
    public static boolean canAutoOpenMimeType(String mimeType) {
        if (ContextUtils.getAppSharedPreferences().getBoolean(
                    HnsPreferenceKeys.HNS_DOWNLOADS_AUTOMATICALLY_OPEN_WHEN_POSSIBLE, true)) {
            return MimeUtils.canAutoOpenMimeType(mimeType);
        }

        return false;
    }
}
