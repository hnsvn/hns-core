/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsToolbarLayoutClassAdapter extends HnsClassVisitor {
    static String sCustomTabToolbarClassName =
            "org/chromium/chrome/browser/customtabs/features/toolbar/CustomTabToolbar";
    static String sToolbarPhoneClassName = "org/chromium/chrome/browser/toolbar/top/ToolbarPhone";
    static String sToolbarTabletClassName = "org/chromium/chrome/browser/toolbar/top/ToolbarTablet";
    static String sHnsToolbarLayoutClassName =
            "org/chromium/chrome/browser/toolbar/top/HnsToolbarLayoutImpl";

    public HnsToolbarLayoutClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sCustomTabToolbarClassName, sHnsToolbarLayoutClassName);

        changeSuperName(sToolbarPhoneClassName, sHnsToolbarLayoutClassName);

        changeSuperName(sToolbarTabletClassName, sHnsToolbarLayoutClassName);

        deleteMethod(sToolbarPhoneClassName, "onHomeButtonUpdate");
    }
}
