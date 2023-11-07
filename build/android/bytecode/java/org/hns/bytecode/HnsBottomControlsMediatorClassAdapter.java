/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsBottomControlsMediatorClassAdapter extends HnsClassVisitor {
    static String sBottomControlsMediatorClassName =
            "org/chromium/chrome/browser/toolbar/bottom/BottomControlsMediator";
    static String sHnsBottomControlsMediatorClassName =
            "org/chromium/chrome/browser/toolbar/bottom/HnsBottomControlsMediator";

    public HnsBottomControlsMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sBottomControlsMediatorClassName, sHnsBottomControlsMediatorClassName);

        deleteField(sHnsBottomControlsMediatorClassName, "mBottomControlsHeight");
        makeProtectedField(sBottomControlsMediatorClassName, "mBottomControlsHeight");

        deleteField(sHnsBottomControlsMediatorClassName, "mModel");
        makeProtectedField(sBottomControlsMediatorClassName, "mModel");

        deleteField(sHnsBottomControlsMediatorClassName, "mBrowserControlsSizer");
        makeProtectedField(sBottomControlsMediatorClassName, "mBrowserControlsSizer");
    }
}
