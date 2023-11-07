/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsTabHelpersClassAdapter extends HnsClassVisitor {
    static String sTabHelpersClassName = "org/chromium/chrome/browser/tab/TabHelpers";
    static String sHnsTabHelpersClassName = "org/chromium/chrome/browser/tab/HnsTabHelpers";

    public HnsTabHelpersClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(sTabHelpersClassName, "initTabHelpers", sHnsTabHelpersClassName);
    }
}
