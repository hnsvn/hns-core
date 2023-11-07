/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsBookmarkPageClassAdapter extends HnsClassVisitor {
    static String sBookmarkPageClassName = "org/chromium/chrome/browser/bookmarks/BookmarkPage";
    static String sHnsBookmarkPageClassName =
            "org/chromium/chrome/browser/bookmarks/HnsBookmarkPage";

    public HnsBookmarkPageClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sBookmarkPageClassName, sHnsBookmarkPageClassName);
        deleteField(sHnsBookmarkPageClassName, "mBookmarkManagerCoordinator");
        makeProtectedField(sBookmarkPageClassName, "mBookmarkManagerCoordinator");
    }
}
