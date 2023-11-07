/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsNewTabPageLayoutClassAdapter extends HnsClassVisitor {
    static String sNewTabPageLayoutClassName = "org/chromium/chrome/browser/ntp/NewTabPageLayout";
    static String sHnsNewTabPageLayoutClassName =
            "org/chromium/chrome/browser/ntp/HnsNewTabPageLayout";
    static String sNewTabPageLayoutSuperClassName = "android/widget/FrameLayout";

    public HnsNewTabPageLayoutClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sHnsNewTabPageLayoutClassName, "mMvTilesContainerLayout");
        makeProtectedField(sNewTabPageLayoutClassName, "mMvTilesContainerLayout");

        deleteField(sHnsNewTabPageLayoutClassName, "mLogoCoordinator");
        makeProtectedField(sNewTabPageLayoutClassName, "mLogoCoordinator");

        makePublicMethod(sNewTabPageLayoutClassName, "insertSiteSectionView");
        addMethodAnnotation(
                sHnsNewTabPageLayoutClassName, "insertSiteSectionView", "Ljava/lang/Override;");

        makePublicMethod(sNewTabPageLayoutClassName, "isScrollableMvtEnabled");
        addMethodAnnotation(
                sHnsNewTabPageLayoutClassName, "isScrollableMvtEnabled", "Ljava/lang/Override;");

        changeSuperName(sNewTabPageLayoutClassName, sNewTabPageLayoutSuperClassName);
    }
}
