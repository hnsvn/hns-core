/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsSettingsLauncherImplClassAdapter extends HnsClassVisitor {
    static String sSettingsLauncherImplClassName =
            "org/chromium/chrome/browser/settings/SettingsLauncherImpl";
    static String sHnsSettingsLauncherImplClassName =
            "org/chromium/chrome/browser/settings/HnsSettingsLauncherImpl";

    public HnsSettingsLauncherImplClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sSettingsLauncherImplClassName, sHnsSettingsLauncherImplClassName);
    }
}
