/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsStartupPaintPreviewHelperClassAdapter extends HnsClassVisitor {
    static String sStartupPaintPreviewHelperClassName =
            "org/chromium/chrome/browser/paint_preview/StartupPaintPreviewHelper";
    static String sHnsStartupPaintPreviewHelperClassName =
            "org/chromium/chrome/browser/paint_preview/HnsStartupPaintPreviewHelper";

    public HnsStartupPaintPreviewHelperClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(sStartupPaintPreviewHelperClassName, "isEnabled",
                sHnsStartupPaintPreviewHelperClassName);
    }
}
