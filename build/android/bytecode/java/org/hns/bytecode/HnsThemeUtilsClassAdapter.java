/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsThemeUtilsClassAdapter extends HnsClassVisitor {
    static String sThemeUtilsClassName = "org/chromium/chrome/browser/theme/ThemeUtils";
    static String sHnsThemeUtilsClassName = "org/chromium/chrome/browser/theme/HnsThemeUtils";

    public HnsThemeUtilsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(sThemeUtilsClassName,
                "getTextBoxColorForToolbarBackgroundInNonNativePage", sHnsThemeUtilsClassName);
    }
}
