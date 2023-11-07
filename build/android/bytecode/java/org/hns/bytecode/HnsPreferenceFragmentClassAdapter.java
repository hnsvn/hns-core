/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsPreferenceFragmentClassAdapter extends HnsClassVisitor {
    static String sDeveloperSettingsClassName =
            "org/chromium/chrome/browser/tracing/settings/DeveloperSettings";

    static String sHnsPreferenceFragmentClassName =
            "org/chromium/chrome/browser/settings/HnsPreferenceFragment";

    public HnsPreferenceFragmentClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sDeveloperSettingsClassName, sHnsPreferenceFragmentClassName);
    }
}
