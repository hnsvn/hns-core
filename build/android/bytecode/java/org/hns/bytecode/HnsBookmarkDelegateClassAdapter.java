/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsBookmarkDelegateClassAdapter extends HnsClassVisitor {
    static String sBookmarkDelegateClassName =
            "org/chromium/chrome/browser/bookmarks/BookmarkDelegate";
    static String sHnsBookmarkDelegateClassName =
            "org/chromium/chrome/browser/bookmarks/HnsBookmarkDelegate";

    public HnsBookmarkDelegateClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sBookmarkDelegateClassName, sHnsBookmarkDelegateClassName);
    }
}
