/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsPermissionDialogDelegateClassAdapter extends HnsClassVisitor {
    static String sPermissionDialogDelegateClassName =
            "org/chromium/components/permissions/PermissionDialogDelegate";

    static String sHnsPermissionDialogDelegateClassName =
            "org/chromium/components/permissions/HnsPermissionDialogDelegate";

    public HnsPermissionDialogDelegateClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(
                sPermissionDialogDelegateClassName, sHnsPermissionDialogDelegateClassName);
    }
}
