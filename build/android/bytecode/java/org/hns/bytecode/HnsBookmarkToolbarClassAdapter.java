/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsBookmarkToolbarClassAdapter extends HnsClassVisitor {
    static String sBookmarkToolbarClassName =
            "org/chromium/chrome/browser/bookmarks/BookmarkToolbar";
    static String sHnsBookmarkToolbarClassName =
            "org/chromium/chrome/browser/bookmarks/HnsBookmarkToolbar";

    public HnsBookmarkToolbarClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sBookmarkToolbarClassName, sHnsBookmarkToolbarClassName);
        deleteField(sHnsBookmarkToolbarClassName, "mBookmarkModel");
        makeProtectedField(sBookmarkToolbarClassName, "mBookmarkModel");
    }
}
