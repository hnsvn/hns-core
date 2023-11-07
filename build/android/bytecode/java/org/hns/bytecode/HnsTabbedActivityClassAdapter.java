/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsTabbedActivityClassAdapter extends HnsClassVisitor {
    static String sChromeTabbedActivityClassName =
            "org/chromium/chrome/browser/ChromeTabbedActivity";
    static String sHnsActivityClassName = "org/chromium/chrome/browser/app/HnsActivity";
    static String sTabbedRootUiCoordinatorClassName =
            "org/chromium/chrome/browser/tabbed_mode/TabbedRootUiCoordinator";
    static String sTabbedAppMenuPropertiesDelegateClassName =
            "org/chromium/chrome/browser/tabbed_mode/TabbedAppMenuPropertiesDelegate";
    static String sHnsTabbedAppMenuPropertiesDelegateClassName =
            "org/chromium/chrome/browser/appmenu/HnsTabbedAppMenuPropertiesDelegate";
    static String sChromeTabCreatorClassName =
            "org/chromium/chrome/browser/tabmodel/ChromeTabCreator";
    static String sHnsTabCreatorClassName =
            "org/chromium/chrome/browser/tabmodel/HnsTabCreator";
    static String sAppMenuPropertiesDelegateImplClassName =
            "org/chromium/chrome/browser/app/appmenu/AppMenuPropertiesDelegateImpl";
    static String sHnsAppMenuPropertiesDelegateImplClassName =
            "org/chromium/chrome/browser/app/appmenu/HnsAppMenuPropertiesDelegateImpl";
    static String sCustomTabAppMenuPropertiesDelegateClassName =
            "org/chromium/chrome/browser/customtabs/CustomTabAppMenuPropertiesDelegate";

    public HnsTabbedActivityClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sChromeTabbedActivityClassName, sHnsActivityClassName);

        changeSuperName(sTabbedAppMenuPropertiesDelegateClassName,
                sHnsAppMenuPropertiesDelegateImplClassName);

        changeSuperName(sCustomTabAppMenuPropertiesDelegateClassName,
                sHnsAppMenuPropertiesDelegateImplClassName);

        redirectConstructor(sTabbedAppMenuPropertiesDelegateClassName,
                sHnsTabbedAppMenuPropertiesDelegateClassName);

        redirectConstructor(sAppMenuPropertiesDelegateImplClassName,
                sHnsAppMenuPropertiesDelegateImplClassName);

        redirectConstructor(sChromeTabCreatorClassName, sHnsTabCreatorClassName);

        makePublicMethod(sChromeTabbedActivityClassName, "hideOverview");

        deleteMethod(sChromeTabbedActivityClassName, "supportsDynamicColors");
    }
}
