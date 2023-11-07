/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsNotificationBuilderClassAdapter extends HnsClassVisitor {
    static String sStandardNotificationBuilderClassName =
            "org/chromium/chrome/browser/notifications/StandardNotificationBuilder";
    static String sHnsNotificationBuilderClassName =
            "org/chromium/chrome/browser/notifications/HnsNotificationBuilder";

    public HnsNotificationBuilderClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sStandardNotificationBuilderClassName, sHnsNotificationBuilderClassName);
    }
}
