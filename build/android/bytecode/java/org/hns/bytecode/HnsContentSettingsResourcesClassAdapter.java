/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsContentSettingsResourcesClassAdapter extends HnsClassVisitor {
    static String sContentSettingsResourcesClassName =
            "org/chromium/components/browser_ui/site_settings/ContentSettingsResources";
    static String sHnsContentSettingsResourcesClassName =
            "org/chromium/components/browser_ui/site_settings/HnsContentSettingsResources";
    static String sContentSettingsResourcesResourceItemClassName =
            "org/chromium/components/browser_ui/site_settings/ContentSettingsResources$ResourceItem";
    static String sHnsContentSettingsResourcesResourceItemClassName =
            "org/chromium/components/browser_ui/site_settings/HnsContentSettingsResources$ResourceItem";

    public HnsContentSettingsResourcesClassAdapter(ClassVisitor visitor) {
        super(visitor);

        makePublicMethod(sContentSettingsResourcesClassName, "getResourceItem");
        changeMethodOwner(sContentSettingsResourcesClassName, "getResourceItem",
                sHnsContentSettingsResourcesClassName);
        makePublicInnerClass(sContentSettingsResourcesClassName, "ResourceItem");
        redirectConstructor(sHnsContentSettingsResourcesResourceItemClassName,
                sContentSettingsResourcesResourceItemClassName);
        redirectTypeInMethod(sHnsContentSettingsResourcesClassName, "getResourceItem",
                sHnsContentSettingsResourcesResourceItemClassName,
                sContentSettingsResourcesResourceItemClassName);
    }
}
