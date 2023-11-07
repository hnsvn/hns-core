/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsLauncherActivityClassAdapter extends HnsClassVisitor {
    static String sChromeLauncherActivityClassName =
            "org/chromium/chrome/browser/document/ChromeLauncherActivity";
    static String sHnsLauncherActivityClassName =
            "org/chromium/chrome/browser/document/HnsLauncherActivity";

    public HnsLauncherActivityClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sChromeLauncherActivityClassName, sHnsLauncherActivityClassName);
    }
}
