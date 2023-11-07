/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsCachedFlagClassAdapter extends HnsClassVisitor {
    static String sCachedFlagClassName = "org/chromium/chrome/browser/flags/CachedFlag";
    static String sHnsCachedFlagClassName = "org/chromium/chrome/browser/flags/HnsCachedFlag";

    public HnsCachedFlagClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sCachedFlagClassName, sHnsCachedFlagClassName);

        deleteField(sHnsCachedFlagClassName, "mDefaultValue");
        makeProtectedField(sCachedFlagClassName, "mDefaultValue");
    }
}
