/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsFeedSurfaceMediatorClassAdapter extends HnsClassVisitor {
    static String sFeedSurfaceMediatorClassName =
            "org/chromium/chrome/browser/feed/FeedSurfaceMediator";
    static String sHnsFeedSurfaceMediatorClassName =
            "org/chromium/chrome/browser/feed/HnsFeedSurfaceMediator";

    public HnsFeedSurfaceMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sFeedSurfaceMediatorClassName, sHnsFeedSurfaceMediatorClassName);

        deleteMethod(sHnsFeedSurfaceMediatorClassName, "destroyPropertiesForStream");
        makePublicMethod(sFeedSurfaceMediatorClassName, "destroyPropertiesForStream");

        deleteField(sHnsFeedSurfaceMediatorClassName, "mCoordinator");
        makeProtectedField(sFeedSurfaceMediatorClassName, "mCoordinator");

        deleteField(sHnsFeedSurfaceMediatorClassName, "mSnapScrollHelper");
        makeProtectedField(sFeedSurfaceMediatorClassName, "mSnapScrollHelper");
    }
}
