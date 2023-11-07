/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsManageAccountDevicesLinkViewClassAdapter extends HnsClassVisitor {
    static String sManageAccountDevicesLinkView =
            "org/chromium/chrome/browser/share/send_tab_to_self/ManageAccountDevicesLinkView";
    static String sHnsManageAccountDevicesLinkView =
            "org/chromium/chrome/browser/share/send_tab_to_self/HnsManageAccountDevicesLinkView";

    public HnsManageAccountDevicesLinkViewClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sManageAccountDevicesLinkView, sHnsManageAccountDevicesLinkView);

        changeMethodOwner(sManageAccountDevicesLinkView, "inflateIfVisible",
                sHnsManageAccountDevicesLinkView);
    }
}
