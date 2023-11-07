/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsBookmarkActivityClassAdapter extends HnsClassVisitor {
    static String sBookmarkActivityClassName =
            "org/chromium/chrome/browser/app/bookmarks/BookmarkActivity";
    static String sHnsBookmarkActivityClassName =
            "org/chromium/chrome/browser/app/bookmarks/HnsBookmarkActivity";

    public HnsBookmarkActivityClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sHnsBookmarkActivityClassName, "mBookmarkManagerCoordinator");
        makeProtectedField(sBookmarkActivityClassName, "mBookmarkManagerCoordinator");
    }
}
