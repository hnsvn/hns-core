/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsBookmarkManagerCoordinatorClassAdapter extends HnsClassVisitor {
    static String sBookmarkManagerCoordinatorClassName =
            "org/chromium/chrome/browser/bookmarks/BookmarkManagerCoordinator";
    static String sHnsBookmarkManagerCoordinatorClassName =
            "org/chromium/chrome/browser/bookmarks/HnsBookmarkManagerCoordinator";

    public HnsBookmarkManagerCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sBookmarkManagerCoordinatorClassName, sHnsBookmarkManagerCoordinatorClassName);
        deleteField(sHnsBookmarkManagerCoordinatorClassName, "mMediator");
        makeProtectedField(sBookmarkManagerCoordinatorClassName, "mMediator");
    }
}
