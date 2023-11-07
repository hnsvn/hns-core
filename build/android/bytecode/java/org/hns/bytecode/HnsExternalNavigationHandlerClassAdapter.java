/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsExternalNavigationHandlerClassAdapter extends HnsClassVisitor {
    static String sExternalNavigationHandlerClassName =
            "org/chromium/components/external_intents/ExternalNavigationHandler";
    static String sHnsHnsExternalNavigationHandlerClassName =
            "org/chromium/chrome/browser/externalnav/HnsExternalNavigationHandler";

    public HnsExternalNavigationHandlerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sExternalNavigationHandlerClassName, sHnsHnsExternalNavigationHandlerClassName);
    }
}
