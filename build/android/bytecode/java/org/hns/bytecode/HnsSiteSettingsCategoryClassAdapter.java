/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsSiteSettingsCategoryClassAdapter extends HnsClassVisitor {
    static String sSiteSettingsCategoryClassName =
            "org/chromium/components/browser_ui/site_settings/SiteSettingsCategory";
    static String sHnsSiteSettingsCategoryClassName =
            "org/chromium/components/browser_ui/site_settings/HnsSiteSettingsCategory";

    public HnsSiteSettingsCategoryClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(sSiteSettingsCategoryClassName, "contentSettingsType",
                sHnsSiteSettingsCategoryClassName);
        changeMethodOwner(sSiteSettingsCategoryClassName, "preferenceKey",
                sHnsSiteSettingsCategoryClassName);
    }
}
