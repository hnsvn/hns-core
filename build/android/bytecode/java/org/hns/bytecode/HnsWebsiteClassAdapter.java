/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsWebsiteClassAdapter extends HnsClassVisitor {
    static String sWebsiteClassName = "org/chromium/components/browser_ui/site_settings/Website";
    static String sHnsWebsiteClassName =
            "org/chromium/components/browser_ui/site_settings/HnsWebsite";

    public HnsWebsiteClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sWebsiteClassName, sHnsWebsiteClassName);

        makePrivateMethod(sWebsiteClassName, "setContentSetting");

        makePublicMethod(sHnsWebsiteClassName, "setContentSetting");
        changeMethodOwner(sWebsiteClassName, "setContentSetting", sHnsWebsiteClassName);
    }
}
