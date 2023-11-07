/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsBookmarkUtilsClassAdapter extends HnsClassVisitor {
    static String sBookmarkUtilsClassName = "org/chromium/chrome/browser/bookmarks/BookmarkUtils";
    static String sHnsBookmarkUtilsClassName =
            "org/chromium/chrome/browser/bookmarks/HnsBookmarkUtils";

    public HnsBookmarkUtilsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(
                sBookmarkUtilsClassName, "addOrEditBookmark", sHnsBookmarkUtilsClassName);
        changeMethodOwner(sBookmarkUtilsClassName, "showBookmarkManagerOnPhone",
                sHnsBookmarkUtilsClassName);
    }
}
