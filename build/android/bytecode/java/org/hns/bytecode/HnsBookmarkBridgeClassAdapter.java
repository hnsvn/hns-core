/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsBookmarkBridgeClassAdapter extends HnsClassVisitor {
    static String sBookmarkBridgeClassName = "org/chromium/chrome/browser/bookmarks/BookmarkBridge";
    static String sHnsBookmarkBridgeClassName =
            "org/chromium/chrome/browser/bookmarks/HnsBookmarkBridge";

    public HnsBookmarkBridgeClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sBookmarkBridgeClassName, sHnsBookmarkBridgeClassName);
        deleteField(sHnsBookmarkBridgeClassName, "mNativeBookmarkBridge");
        makeProtectedField(sBookmarkBridgeClassName, "mNativeBookmarkBridge");
    }
}
