/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsFeedSurfaceCoordinatorClassAdapter extends HnsClassVisitor {
    static String sFeedSurfaceCoordinatorClassName =
            "org/chromium/chrome/browser/feed/FeedSurfaceCoordinator";
    static String sHnsFeedSurfaceCoordinatorClassName =
            "org/chromium/chrome/browser/feed/HnsFeedSurfaceCoordinator";

    public HnsFeedSurfaceCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sHnsFeedSurfaceCoordinatorClassName, "mNtpHeader");
        makeProtectedField(sFeedSurfaceCoordinatorClassName, "mNtpHeader");

        deleteField(sHnsFeedSurfaceCoordinatorClassName, "mRootView");
        makeProtectedField(sFeedSurfaceCoordinatorClassName, "mRootView");
    }
}
