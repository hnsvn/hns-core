/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsToolbarManagerClassAdapter extends HnsClassVisitor {
    static String sToolbarManagerClassName = "org/chromium/chrome/browser/toolbar/ToolbarManager";
    static String sHnsToolbarManagerClassName =
            "org/chromium/chrome/browser/toolbar/HnsToolbarManager";

    public HnsToolbarManagerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sToolbarManagerClassName, sHnsToolbarManagerClassName);

        deleteField(sHnsToolbarManagerClassName, "mBottomControlsCoordinatorSupplier");
        makeProtectedField(sToolbarManagerClassName, "mBottomControlsCoordinatorSupplier");

        deleteField(sHnsToolbarManagerClassName, "mCallbackController");
        makeProtectedField(sToolbarManagerClassName, "mCallbackController");

        deleteField(sHnsToolbarManagerClassName, "mBrowserControlsSizer");
        makeProtectedField(sToolbarManagerClassName, "mBrowserControlsSizer");

        deleteField(sHnsToolbarManagerClassName, "mFullscreenManager");
        makeProtectedField(sToolbarManagerClassName, "mFullscreenManager");

        deleteField(sHnsToolbarManagerClassName, "mActivityTabProvider");
        makeProtectedField(sToolbarManagerClassName, "mActivityTabProvider");

        deleteField(sHnsToolbarManagerClassName, "mAppThemeColorProvider");
        makeProtectedField(sToolbarManagerClassName, "mAppThemeColorProvider");

        deleteField(sHnsToolbarManagerClassName, "mScrimCoordinator");
        makeProtectedField(sToolbarManagerClassName, "mScrimCoordinator");

        deleteField(sHnsToolbarManagerClassName, "mShowStartSurfaceSupplier");
        makeProtectedField(sToolbarManagerClassName, "mShowStartSurfaceSupplier");

        deleteField(sHnsToolbarManagerClassName, "mMenuButtonCoordinator");
        makeProtectedField(sToolbarManagerClassName, "mMenuButtonCoordinator");

        deleteField(sHnsToolbarManagerClassName, "mToolbarTabController");
        makeProtectedField(sToolbarManagerClassName, "mToolbarTabController");

        deleteField(sHnsToolbarManagerClassName, "mLocationBar");
        makeProtectedField(sToolbarManagerClassName, "mLocationBar");

        deleteField(sHnsToolbarManagerClassName, "mActionModeController");
        makeProtectedField(sToolbarManagerClassName, "mActionModeController");

        deleteField(sHnsToolbarManagerClassName, "mLocationBarModel");
        makeProtectedField(sToolbarManagerClassName, "mLocationBarModel");

        deleteField(sHnsToolbarManagerClassName, "mToolbar");
        makeProtectedField(sToolbarManagerClassName, "mToolbar");

        deleteField(sHnsToolbarManagerClassName, "mBookmarkModelSupplier");
        makeProtectedField(sToolbarManagerClassName, "mBookmarkModelSupplier");

        deleteField(sHnsToolbarManagerClassName, "mLayoutManager");
        makeProtectedField(sToolbarManagerClassName, "mLayoutManager");

        deleteField(sHnsToolbarManagerClassName, "mOverlayPanelVisibilitySupplier");
        makeProtectedField(sToolbarManagerClassName, "mOverlayPanelVisibilitySupplier");

        deleteField(sHnsToolbarManagerClassName, "mTabModelSelector");
        makeProtectedField(sToolbarManagerClassName, "mTabModelSelector");

        deleteField(sHnsToolbarManagerClassName, "mIncognitoStateProvider");
        makeProtectedField(sToolbarManagerClassName, "mIncognitoStateProvider");

        deleteField(sHnsToolbarManagerClassName, "mTabCountProvider");
        makeProtectedField(sToolbarManagerClassName, "mTabCountProvider");

        deleteField(sHnsToolbarManagerClassName, "mTabGroupUi");
        makeProtectedField(sToolbarManagerClassName, "mTabGroupUi");

        deleteField(sHnsToolbarManagerClassName, "mBottomSheetController");
        makeProtectedField(sToolbarManagerClassName, "mBottomSheetController");

        deleteField(sHnsToolbarManagerClassName, "mActivityLifecycleDispatcher");
        makeProtectedField(sToolbarManagerClassName, "mActivityLifecycleDispatcher");

        deleteField(sHnsToolbarManagerClassName, "mIsWarmOnResumeSupplier");
        makeProtectedField(sToolbarManagerClassName, "mIsWarmOnResumeSupplier");

        deleteField(sHnsToolbarManagerClassName, "mTabContentManager");
        makeProtectedField(sToolbarManagerClassName, "mTabContentManager");

        deleteField(sHnsToolbarManagerClassName, "mTabCreatorManager");
        makeProtectedField(sToolbarManagerClassName, "mTabCreatorManager");

        deleteField(sHnsToolbarManagerClassName, "mSnackbarManager");
        makeProtectedField(sToolbarManagerClassName, "mSnackbarManager");

        deleteField(sHnsToolbarManagerClassName, "mTabObscuringHandler");
        makeProtectedField(sToolbarManagerClassName, "mTabObscuringHandler");

        makePublicMethod(sToolbarManagerClassName, "onOrientationChange");
        addMethodAnnotation(
                sHnsToolbarManagerClassName, "onOrientationChange", "Ljava/lang/Override;");

        makePublicMethod(sToolbarManagerClassName, "updateBookmarkButtonStatus");
        addMethodAnnotation(sHnsToolbarManagerClassName, "updateBookmarkButtonStatus",
                "Ljava/lang/Override;");

        makePublicMethod(sToolbarManagerClassName, "updateReloadState");
        deleteMethod(sHnsToolbarManagerClassName, "updateReloadState");
    }
}
