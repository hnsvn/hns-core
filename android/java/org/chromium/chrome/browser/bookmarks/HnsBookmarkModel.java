/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.bookmarks;

import org.chromium.ui.base.WindowAndroid;

public class HnsBookmarkModel extends BookmarkModel {
    HnsBookmarkModel(long nativeBookmarkBridge) {
        super(nativeBookmarkBridge);
    }

    public void importBookmarks(WindowAndroid windowAndroid, String importFilePath) {
        assert false : "importBookmarks should be redirected to HnsBookmarkBridge in bytecode!";
    }

    public void exportBookmarks(WindowAndroid windowAndroid, String exportFilePath) {
        assert false : "exportBookmarks should be redirected to HnsBookmarkBridge in bytecode!";
    }
}
