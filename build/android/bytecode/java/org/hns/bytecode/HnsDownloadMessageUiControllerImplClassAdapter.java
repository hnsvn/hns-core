/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsDownloadMessageUiControllerImplClassAdapter extends HnsClassVisitor {
    static String sDownloadMessageUiControllerImpl =
            "org/chromium/chrome/browser/download/DownloadMessageUiControllerImpl";

    static String sHnsDownloadMessageUiControllerImpl =
            "org/chromium/chrome/browser/download/HnsDownloadMessageUiControllerImpl";

    public HnsDownloadMessageUiControllerImplClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sDownloadMessageUiControllerImpl, sHnsDownloadMessageUiControllerImpl);
        changeMethodOwner(sDownloadMessageUiControllerImpl, "isVisibleToUser",
                sHnsDownloadMessageUiControllerImpl);
    }
}
