/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsFreIntentCreatorClassAdapter extends HnsClassVisitor {
    static String sFreIntentCreatorClassName =
            "org/chromium/chrome/browser/firstrun/FreIntentCreator";

    static String sHnsFreIntentCreatorClassName =
            "org/chromium/chrome/browser/firstrun/HnsFreIntentCreator";

    public HnsFreIntentCreatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sFreIntentCreatorClassName, sHnsFreIntentCreatorClassName);
    }
}
