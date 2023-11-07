/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsCustomizationProviderDelegateImplClassAdapter extends HnsClassVisitor {
    static String sCustomizationProviderDelegateImplClassName =
            "org/chromium/chrome/browser/partnercustomizations/CustomizationProviderDelegateImpl";
    static String sHnsCustomizationProviderDelegateImplClassName =
            "org/chromium/chrome/browser/partnercustomizations/HnsCustomizationProviderDelegateImpl";

    public HnsCustomizationProviderDelegateImplClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sCustomizationProviderDelegateImplClassName,
                sHnsCustomizationProviderDelegateImplClassName);
    }
}
