/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsShareDelegateImplClassAdapter extends HnsClassVisitor {
    static String sShareDelegateImplClassName =
            "org/chromium/chrome/browser/share/ShareDelegateImpl";
    static String sHnsShareDelegateImplClassName =
            "org/chromium/chrome/browser/share/HnsShareDelegateImpl";

    public HnsShareDelegateImplClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sShareDelegateImplClassName, sHnsShareDelegateImplClassName);
    }
}
