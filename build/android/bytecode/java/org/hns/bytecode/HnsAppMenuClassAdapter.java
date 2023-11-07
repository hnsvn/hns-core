/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsAppMenuClassAdapter extends HnsClassVisitor {
    static String sAppMenuClassName = "org/chromium/chrome/browser/ui/appmenu/AppMenu";

    static String sHnsAppMenuClassName = "org/chromium/chrome/browser/ui/appmenu/HnsAppMenu";

    public HnsAppMenuClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sAppMenuClassName, sHnsAppMenuClassName);

        changeMethodOwner(sAppMenuClassName, "getPopupPosition", sHnsAppMenuClassName);

        makePublicMethod(sAppMenuClassName, "runMenuItemEnterAnimations");
        addMethodAnnotation(
                sHnsAppMenuClassName, "runMenuItemEnterAnimations", "Ljava/lang/Override;");
    }
}
