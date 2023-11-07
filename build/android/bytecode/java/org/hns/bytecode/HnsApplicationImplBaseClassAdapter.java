/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsApplicationImplBaseClassAdapter extends HnsClassVisitor {
    static String sChromeApplicationImplClassName =
            "org/chromium/chrome/browser/ChromeApplicationImpl";

    static String sHnsApplicationImplBaseClassName =
            "org/chromium/chrome/browser/HnsApplicationImplBase";

    public HnsApplicationImplBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sChromeApplicationImplClassName, sHnsApplicationImplBaseClassName);
    }
}
