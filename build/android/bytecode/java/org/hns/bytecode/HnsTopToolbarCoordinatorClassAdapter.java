/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsTopToolbarCoordinatorClassAdapter extends HnsClassVisitor {
    static String sTopToolbarCoordinatorClassName =
            "org/chromium/chrome/browser/toolbar/top/TopToolbarCoordinator";
    static String sHnsTopToolbarCoordinatorClassName =
            "org/chromium/chrome/browser/toolbar/top/HnsTopToolbarCoordinator";

    public HnsTopToolbarCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sTopToolbarCoordinatorClassName, sHnsTopToolbarCoordinatorClassName);

        deleteField(sHnsTopToolbarCoordinatorClassName, "mTabSwitcherModeCoordinator");
        makeProtectedField(sTopToolbarCoordinatorClassName, "mTabSwitcherModeCoordinator");

        deleteField(sHnsTopToolbarCoordinatorClassName, "mOptionalButtonController");
        makeProtectedField(sTopToolbarCoordinatorClassName, "mOptionalButtonController");

        deleteField(sHnsTopToolbarCoordinatorClassName, "mToolbarColorObserverManager");
        makeProtectedField(sTopToolbarCoordinatorClassName, "mToolbarColorObserverManager");
    }
}
