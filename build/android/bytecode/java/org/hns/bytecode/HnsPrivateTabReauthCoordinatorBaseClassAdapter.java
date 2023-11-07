/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsPrivateTabReauthCoordinatorBaseClassAdapter extends HnsClassVisitor {
    static String sTabSwitcherIncognitoReauthCoordinatorClassName =
            "org/chromium/chrome/browser/incognito/reauth/TabSwitcherIncognitoReauthCoordinator";

    static String sFullScreenIncognitoReauthCoordinatorClassName =
            "org/chromium/chrome/browser/incognito/reauth/FullScreenIncognitoReauthCoordinator";

    static String sIncognitoReauthCoordinatorBaseClassName =
            "org/chromium/chrome/browser/incognito/reauth/IncognitoReauthCoordinatorBase";

    static String sHnsPrivateTabReauthCoordinatorBaseClassName =
            "org/chromium/chrome/browser/incognito/reauth/HnsPrivateTabReauthCoordinatorBase";

    public HnsPrivateTabReauthCoordinatorBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sTabSwitcherIncognitoReauthCoordinatorClassName,
                sHnsPrivateTabReauthCoordinatorBaseClassName);

        changeSuperName(sFullScreenIncognitoReauthCoordinatorClassName,
                sHnsPrivateTabReauthCoordinatorBaseClassName);

        deleteField(sHnsPrivateTabReauthCoordinatorBaseClassName, "mIncognitoReauthView");
        makeProtectedField(sIncognitoReauthCoordinatorBaseClassName, "mIncognitoReauthView");
    }
}
