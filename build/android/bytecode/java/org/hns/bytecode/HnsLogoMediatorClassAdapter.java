/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsLogoMediatorClassAdapter extends HnsClassVisitor {
    static String sLogoMediator = "org/chromium/chrome/browser/logo/LogoMediator";
    static String sHnsLogoMediator = "org/chromium/chrome/browser/logo/HnsLogoMediator";

    public HnsLogoMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sLogoMediator, sHnsLogoMediator);

        deleteField(sHnsLogoMediator, "mLogoModel");
        makeProtectedField(sLogoMediator, "mLogoModel");

        deleteField(sHnsLogoMediator, "mShouldShowLogo");
        makeProtectedField(sLogoMediator, "mShouldShowLogo");

        makePublicMethod(sLogoMediator, "updateVisibility");
        addMethodAnnotation(sHnsLogoMediator, "updateVisibility", "Ljava/lang/Override;");
    }
}
