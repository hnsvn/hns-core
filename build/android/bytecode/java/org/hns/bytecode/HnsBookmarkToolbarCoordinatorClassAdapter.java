/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsBookmarkToolbarCoordinatorClassAdapter extends HnsClassVisitor {
    static String sBookmarkToolbarCoordinatorClassName =
            "org/chromium/chrome/browser/bookmarks/BookmarkToolbarCoordinator";
    static String sHnsBookmarkToolbarCoordinatorClassName =
            "org/chromium/chrome/browser/bookmarks/HnsBookmarkToolbarCoordinator";

    public HnsBookmarkToolbarCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sBookmarkToolbarCoordinatorClassName, sHnsBookmarkToolbarCoordinatorClassName);

        deleteField(sHnsBookmarkToolbarCoordinatorClassName, "mToolbar");
        makeProtectedField(sBookmarkToolbarCoordinatorClassName, "mToolbar");
    }
}
