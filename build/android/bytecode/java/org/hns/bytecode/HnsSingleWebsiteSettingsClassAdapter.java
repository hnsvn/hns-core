/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsSingleWebsiteSettingsClassAdapter extends HnsClassVisitor {
    static String sSingleWebsiteSettingsClassName =
            "org/chromium/components/browser_ui/site_settings/SingleWebsiteSettings";
    static String sHnsSingleWebsiteSettingsClassName =
            "org/chromium/components/browser_ui/site_settings/HnsSingleWebsiteSettings";

    public HnsSingleWebsiteSettingsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sSingleWebsiteSettingsClassName, sHnsSingleWebsiteSettingsClassName);

        changeMethodOwner(sSingleWebsiteSettingsClassName, "getPreferenceKey",
                sHnsSingleWebsiteSettingsClassName);
        changeMethodOwner(sSingleWebsiteSettingsClassName, "setupContentSettingsPreferences",
                sHnsSingleWebsiteSettingsClassName);

        makePublicMethod(sSingleWebsiteSettingsClassName, "setupContentSettingsPreference");
        changeMethodOwner(sHnsSingleWebsiteSettingsClassName, "setupContentSettingsPreference",
                sSingleWebsiteSettingsClassName);
    }
}
