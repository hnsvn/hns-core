/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsIncognitoToggleTabLayoutClassAdapter extends HnsClassVisitor {
    static String sIncognitoToggleTabLayoutClassName =
            "org/chromium/chrome/browser/toolbar/IncognitoToggleTabLayout";
    static String sHnsIncognitoToggleTabLayoutClassName =
            "org/chromium/chrome/browser/toolbar/HnsIncognitoToggleTabLayout";

    public HnsIncognitoToggleTabLayoutClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sHnsIncognitoToggleTabLayoutClassName, "mIncognitoButtonIcon");
        makeProtectedField(sIncognitoToggleTabLayoutClassName, "mIncognitoButtonIcon");
    }
}
