/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsNewTabPageClassAdapter extends HnsClassVisitor {
    static String sNewTabPageClassName = "org/chromium/chrome/browser/ntp/NewTabPage";
    static String sHnsNewTabPageClassName = "org/chromium/chrome/browser/ntp/HnsNewTabPage";

    public HnsNewTabPageClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sNewTabPageClassName, sHnsNewTabPageClassName);

        deleteField(sHnsNewTabPageClassName, "mBrowserControlsStateProvider");
        makeProtectedField(sNewTabPageClassName, "mBrowserControlsStateProvider");

        deleteField(sHnsNewTabPageClassName, "mNewTabPageLayout");
        makeProtectedField(sNewTabPageClassName, "mNewTabPageLayout");

        deleteField(sHnsNewTabPageClassName, "mFeedSurfaceProvider");
        makeProtectedField(sNewTabPageClassName, "mFeedSurfaceProvider");

        deleteField(sHnsNewTabPageClassName, "mToolbarSupplier");
        makeProtectedField(sNewTabPageClassName, "mToolbarSupplier");

        deleteField(sHnsNewTabPageClassName, "mTabModelSelector");
        makeProtectedField(sNewTabPageClassName, "mTabModelSelector");

        deleteField(sHnsNewTabPageClassName, "mBottomSheetController");
        makeProtectedField(sNewTabPageClassName, "mBottomSheetController");

        makePublicMethod(sNewTabPageClassName, "updateSearchProviderHasLogo");
        addMethodAnnotation(
                sHnsNewTabPageClassName, "updateSearchProviderHasLogo", "Ljava/lang/Override;");
    }
}
