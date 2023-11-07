/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsBookmarkModelClassAdapter extends HnsClassVisitor {
    static String sHnsBookmarkBridgeClassName =
            "org/chromium/chrome/browser/bookmarks/HnsBookmarkBridge";
    static String sBookmarkModelClassName = "org/chromium/chrome/browser/bookmarks/BookmarkModel";
    static String sHnsBookmarkModelClassName =
            "org/chromium/chrome/browser/bookmarks/HnsBookmarkModel";

    public HnsBookmarkModelClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sBookmarkModelClassName, sHnsBookmarkBridgeClassName);
        redirectConstructor(sBookmarkModelClassName, sHnsBookmarkModelClassName);
        deleteMethod(sHnsBookmarkModelClassName, "importBookmarks");
        deleteMethod(sHnsBookmarkModelClassName, "exportBookmarks");
    }
}
