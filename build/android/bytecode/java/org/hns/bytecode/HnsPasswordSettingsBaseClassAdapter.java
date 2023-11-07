/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsPasswordSettingsBaseClassAdapter extends HnsClassVisitor {
    static String sPasswordSettingsClassName =
            "org/chromium/chrome/browser/password_manager/settings/PasswordSettings";
    static String sHnsPasswordSettingsBaseClassName =
            "org/chromium/chrome/browser/password_manager/settings/HnsPasswordSettingsBase";

    public HnsPasswordSettingsBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sPasswordSettingsClassName, sHnsPasswordSettingsBaseClassName);

        changeMethodOwner(sPasswordSettingsClassName, "createCheckPasswords",
                sHnsPasswordSettingsBaseClassName);
        deleteMethod(sPasswordSettingsClassName, "createCheckPasswords");
    }
}
