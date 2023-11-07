/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsSiteSettingsDelegateClassAdapter extends HnsClassVisitor {

    static String sChromeSiteSettingsDelegateClassName = "org/chromium/chrome/browser/site_settings/ChromeSiteSettingsDelegate";
    static String sHnsSiteSettingsDelegateClassName = "org/chromium/chrome/browser/site_settings/HnsSiteSettingsDelegate";

    public HnsSiteSettingsDelegateClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sChromeSiteSettingsDelegateClassName, sHnsSiteSettingsDelegateClassName);
    }
}
