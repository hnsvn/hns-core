/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsDefaultBrowserPromoUtilsClassAdapter extends HnsClassVisitor {
    static String sDefaultBrowserPromoUtilsClassName =
            "org/chromium/chrome/browser/ui/default_browser_promo/DefaultBrowserPromoUtils";

    static String sHnsDefaultBrowserPromoUtilsClassName =
            "org/chromium/chrome/browser/ui/default_browser_promo/HnsDefaultBrowserPromoUtils";

    public HnsDefaultBrowserPromoUtilsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(sDefaultBrowserPromoUtilsClassName, "prepareLaunchPromoIfNeeded",
                sHnsDefaultBrowserPromoUtilsClassName);
    }
}
