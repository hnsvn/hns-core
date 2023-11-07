/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsDynamicColorsClassAdapter extends HnsClassVisitor {
    static String sDynamicColorsClassName = "com/google/android/material/color/DynamicColors";

    static String sHnsDynamicColorsClassName =
            "org/chromium/chrome/browser/util/HnsDynamicColors";

    public HnsDynamicColorsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(sDynamicColorsClassName, "applyToActivityIfAvailable",
                sHnsDynamicColorsClassName);
    }
}
