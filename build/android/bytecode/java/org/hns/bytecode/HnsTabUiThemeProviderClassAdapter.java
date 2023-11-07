/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsTabUiThemeProviderClassAdapter extends HnsClassVisitor {
    static String sTabUiThemeProviderClassName =
            "org/chromium/chrome/browser/tasks/tab_management/TabUiThemeProvider";
    static String sHnsTabUiThemeProviderClassName =
            "org/chromium/chrome/browser/tasks/tab_management/HnsTabUiThemeProvider";

    public HnsTabUiThemeProviderClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(sTabUiThemeProviderClassName, "getTitleTextColor",
                sHnsTabUiThemeProviderClassName);

        changeMethodOwner(sTabUiThemeProviderClassName, "getActionButtonTintList",
                sHnsTabUiThemeProviderClassName);

        changeMethodOwner(sTabUiThemeProviderClassName, "getCardViewBackgroundColor",
                sHnsTabUiThemeProviderClassName);
    }
}
