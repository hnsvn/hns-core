/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsIntentHandlerClassAdapter extends HnsClassVisitor {
    static String sIntentHandlerClassName = "org/chromium/chrome/browser/IntentHandler";
    static String sHnsIntentHandlerClassName = "org/chromium/chrome/browser/HnsIntentHandler";

    public HnsIntentHandlerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sIntentHandlerClassName, sHnsIntentHandlerClassName);

        makePublicMethod(sIntentHandlerClassName, "getUrlForCustomTab");
        changeMethodOwner(
                sHnsIntentHandlerClassName, "getUrlForCustomTab", sIntentHandlerClassName);

        makePublicMethod(sIntentHandlerClassName, "getUrlForWebapp");
        changeMethodOwner(sHnsIntentHandlerClassName, "getUrlForWebapp", sIntentHandlerClassName);

        makePublicMethod(sIntentHandlerClassName, "isJavascriptSchemeOrInvalidUrl");
        changeMethodOwner(sHnsIntentHandlerClassName, "isJavascriptSchemeOrInvalidUrl",
                sIntentHandlerClassName);

        changeMethodOwner(
                sIntentHandlerClassName, "extractUrlFromIntent", sHnsIntentHandlerClassName);
    }
}
