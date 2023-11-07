/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsTabSwitcherModeTopToolbarClassAdapter extends HnsClassVisitor {
    static String sTabSwitcherModeTopToolbarClassName =
            "org/chromium/chrome/browser/toolbar/top/TabSwitcherModeTopToolbar";
    static String sHnsTabSwitcherModeTopToolbarClassName =
            "org/chromium/chrome/browser/toolbar/top/HnsTabSwitcherModeTopToolbar";

    public HnsTabSwitcherModeTopToolbarClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sHnsTabSwitcherModeTopToolbarClassName, "mNewTabViewButton");
        makeProtectedField(sTabSwitcherModeTopToolbarClassName, "mNewTabViewButton");

        deleteField(sHnsTabSwitcherModeTopToolbarClassName, "mNewTabImageButton");
        makeProtectedField(sTabSwitcherModeTopToolbarClassName, "mNewTabImageButton");

        deleteField(sHnsTabSwitcherModeTopToolbarClassName, "mShouldShowNewTabVariation");
        makeProtectedField(sTabSwitcherModeTopToolbarClassName, "mShouldShowNewTabVariation");

        deleteField(sHnsTabSwitcherModeTopToolbarClassName, "mIsIncognito");
        makeProtectedField(sTabSwitcherModeTopToolbarClassName, "mIsIncognito");

        makePublicMethod(sTabSwitcherModeTopToolbarClassName, "updateNewTabButtonVisibility");
        addMethodAnnotation(sHnsTabSwitcherModeTopToolbarClassName,
                "updateNewTabButtonVisibility", "Ljava/lang/Override;");

        makePublicMethod(sTabSwitcherModeTopToolbarClassName, "getToolbarColorForCurrentState");
        addMethodAnnotation(sHnsTabSwitcherModeTopToolbarClassName,
                "getToolbarColorForCurrentState", "Ljava/lang/Override;");

        makePublicMethod(sTabSwitcherModeTopToolbarClassName, "shouldShowIncognitoToggle");
        deleteMethod(sHnsTabSwitcherModeTopToolbarClassName, "shouldShowIncognitoToggle");
    }
}
