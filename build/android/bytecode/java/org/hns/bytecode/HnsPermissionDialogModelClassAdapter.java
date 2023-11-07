/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsPermissionDialogModelClassAdapter extends HnsClassVisitor {
    static String sPermissionDialogModelClassName =
            "org/chromium/components/permissions/PermissionDialogModel";

    static String sHnsPermissionDialogModelClassName =
            "org/chromium/components/permissions/HnsPermissionDialogModel";

    public HnsPermissionDialogModelClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeMethodOwner(
                sPermissionDialogModelClassName, "getModel", sHnsPermissionDialogModelClassName);
    }
}
