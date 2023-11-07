/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.search_engines.settings;

import org.chromium.base.ThreadUtils;
import org.chromium.base.annotations.NativeMethods;

public class HnsSearchEnginePrefHelper {
    private static HnsSearchEnginePrefHelper sInstance;

    private HnsSearchEnginePrefHelper() {}

    public static HnsSearchEnginePrefHelper getInstance() {
        ThreadUtils.assertOnUiThread();
        if (sInstance == null) {
            sInstance = new HnsSearchEnginePrefHelper();
        }
        return sInstance;
    }

    public void setFetchSEFromNative(boolean value) {
        HnsSearchEnginePrefHelperJni.get().setFetchSEFromNative(value);
    }

    public boolean getFetchSEFromNative() {
        return HnsSearchEnginePrefHelperJni.get().getFetchSEFromNative();
    }

    @NativeMethods
    interface Natives {
        void setFetchSEFromNative(boolean value);
        boolean getFetchSEFromNative();
    }
}
