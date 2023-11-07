/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsNotificationManagerProxyImplClassAdapter extends HnsClassVisitor {
    static String sNotificationManagerProxyImplClassName =
            "org/chromium/components/browser_ui/notifications/NotificationManagerProxyImpl";
    static String sHnsNotificationManagerProxyImplClassName =
            "org/chromium/chrome/browser/notifications/HnsNotificationManagerProxyImpl";

    public HnsNotificationManagerProxyImplClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sNotificationManagerProxyImplClassName,
                sHnsNotificationManagerProxyImplClassName);
    }
}
