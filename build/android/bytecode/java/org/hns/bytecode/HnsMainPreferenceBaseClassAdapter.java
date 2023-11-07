/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsMainPreferenceBaseClassAdapter extends HnsClassVisitor {
    static String sMainPreferencesClassName =
            "org/chromium/chrome/browser/settings/MainSettings";

    static String sHnsMainPreferencesBaseClassName =
            "org/chromium/chrome/browser/settings/HnsMainPreferencesBase";

    public HnsMainPreferenceBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sMainPreferencesClassName,
                        sHnsMainPreferencesBaseClassName);
    }
}
