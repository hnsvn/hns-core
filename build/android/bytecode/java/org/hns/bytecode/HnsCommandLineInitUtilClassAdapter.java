/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsCommandLineInitUtilClassAdapter extends HnsClassVisitor {
    static String sCommandLineClassName =
        "org/chromium/base/CommandLineInitUtil";

    static String sHnsCommandLineClassName =
        "org/chromium/base/HnsCommandLineInitUtil";

    static String sMethodReadFileAsUtf8 = "initCommandLine";

    public HnsCommandLineInitUtilClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeMethodOwner(sCommandLineClassName, sMethodReadFileAsUtf8,
                          sHnsCommandLineClassName);
    }
}
