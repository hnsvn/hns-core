/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsLaunchIntentDispatcherClassAdapter extends HnsClassVisitor {
    static String sLaunchIntentDispatcherClassName = "org/chromium/chrome/browser/LaunchIntentDispatcher";
    static String sHnsLaunchIntentDispatcherClassName = "org/chromium/chrome/browser/HnsLaunchIntentDispatcher";

    public HnsLaunchIntentDispatcherClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeMethodOwner(sLaunchIntentDispatcherClassName, "isCustomTabIntent", sHnsLaunchIntentDispatcherClassName);
    }
}
