/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsLocationBarMediatorClassAdapter extends HnsClassVisitor {
    static String sLocationBarMediator = "org/chromium/chrome/browser/omnibox/LocationBarMediator";
    static String sHnsLocationBarMediator =
            "org/chromium/chrome/browser/omnibox/HnsLocationBarMediator";

    public HnsLocationBarMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sLocationBarMediator, sHnsLocationBarMediator);

        deleteMethod(sHnsLocationBarMediator, "shouldShowDeleteButton");
        makePublicMethod(sLocationBarMediator, "shouldShowDeleteButton");

        deleteField(sHnsLocationBarMediator, "mNativeInitialized");
        makeProtectedField(sLocationBarMediator, "mNativeInitialized");
        deleteField(sHnsLocationBarMediator, "mWindowAndroid");
        makeProtectedField(sLocationBarMediator, "mWindowAndroid");
        deleteField(sHnsLocationBarMediator, "mLocationBarLayout");
        makeProtectedField(sLocationBarMediator, "mLocationBarLayout");
        deleteField(sHnsLocationBarMediator, "mIsUrlFocusChangeInProgress");
        makeProtectedField(sLocationBarMediator, "mIsUrlFocusChangeInProgress");
        deleteField(sHnsLocationBarMediator, "mUrlHasFocus");
        makeProtectedField(sLocationBarMediator, "mUrlHasFocus");
        deleteField(sHnsLocationBarMediator, "mIsTablet");
        makeProtectedField(sLocationBarMediator, "mIsTablet");
        deleteField(sHnsLocationBarMediator, "mIsLocationBarFocusedFromNtpScroll");
        makeProtectedField(sLocationBarMediator, "mIsLocationBarFocusedFromNtpScroll");
        deleteField(sHnsLocationBarMediator, "mShouldClearOmniboxOnFocus");
        makeProtectedField(sLocationBarMediator, "mShouldClearOmniboxOnFocus");
        deleteField(sHnsLocationBarMediator, "mContext");
        makeProtectedField(sLocationBarMediator, "mContext");
        deleteField(sHnsLocationBarMediator, "mBrandedColorScheme");
        makeProtectedField(sLocationBarMediator, "mBrandedColorScheme");
        deleteField(sHnsLocationBarMediator, "mAssistantVoiceSearchServiceSupplier");
        makeProtectedField(sLocationBarMediator, "mAssistantVoiceSearchServiceSupplier");
    }
}
