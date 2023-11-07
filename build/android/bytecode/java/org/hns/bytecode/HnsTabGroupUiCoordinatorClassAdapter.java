/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsTabGroupUiCoordinatorClassAdapter extends HnsClassVisitor {
    static String sTabGroupUiCoordinatorClassName =
            "org/chromium/chrome/browser/tasks/tab_management/TabGroupUiCoordinator";
    static String sHnsTabGroupUiCoordinatorClassName =
            "org/chromium/chrome/browser/tasks/tab_management/HnsTabGroupUiCoordinator";

    public HnsTabGroupUiCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sTabGroupUiCoordinatorClassName, sHnsTabGroupUiCoordinatorClassName);

        deleteField(sHnsTabGroupUiCoordinatorClassName, "mToolbarView");
        makeProtectedField(sTabGroupUiCoordinatorClassName, "mToolbarView");
    }
}
