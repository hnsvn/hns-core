/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsNotificationPermissionRationaleDialogControllerClassAdapter
        extends HnsClassVisitor {
    static String sContoller =
            "org/chromium/chrome/browser/notifications/permissions/NotificationPermissionRationaleDialogController";
    static String sHnsContoller =
            "org/chromium/chrome/browser/notifications/permissions/HnsNotificationPermissionRationaleDialogController";

    public HnsNotificationPermissionRationaleDialogControllerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sContoller, sHnsContoller);
        deleteMethod(sHnsContoller, "wrapDialogDismissalCallback");
        makePublicMethod(sContoller, "wrapDialogDismissalCallback");
    }
}
