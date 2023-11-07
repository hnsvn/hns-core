/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsBookmarkManagerMediatorClassAdapter extends HnsClassVisitor {
    static String sBookmarkManagerMediatorClassName =
            "org/chromium/chrome/browser/bookmarks/BookmarkManagerMediator";
    static String sHnsBookmarkManagerMediatorClassName =
            "org/chromium/chrome/browser/bookmarks/HnsBookmarkManagerMediator";

    public HnsBookmarkManagerMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sBookmarkManagerMediatorClassName, sHnsBookmarkManagerMediatorClassName);

        deleteField(sHnsBookmarkManagerMediatorClassName, "mBookmarkModel");
        makeProtectedField(sBookmarkManagerMediatorClassName, "mBookmarkModel");
        deleteField(sHnsBookmarkManagerMediatorClassName, "mContext");
        makeProtectedField(sBookmarkManagerMediatorClassName, "mContext");
    }
}
