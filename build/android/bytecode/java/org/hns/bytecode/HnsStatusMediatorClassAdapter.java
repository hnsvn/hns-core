/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsStatusMediatorClassAdapter extends HnsClassVisitor {
    static String sStatusMediatorClassName =
            "org/chromium/chrome/browser/omnibox/status/StatusMediator";
    static String sHnsStatusMediatorClassName =
            "org/chromium/chrome/browser/omnibox/status/HnsStatusMediator";

    public HnsStatusMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sStatusMediatorClassName, sHnsStatusMediatorClassName);

        deleteField(sHnsStatusMediatorClassName, "mUrlHasFocus");
        makeProtectedField(sStatusMediatorClassName, "mUrlHasFocus");
    }
}
