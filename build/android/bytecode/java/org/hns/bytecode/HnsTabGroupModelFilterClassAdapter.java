/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsTabGroupModelFilterClassAdapter extends HnsClassVisitor {
    static String sTabGroupModelFilterClassName =
            "org/chromium/chrome/browser/tasks/tab_groups/TabGroupModelFilter";
    static String sHnsTabGroupModelFilterClassName =
            "org/chromium/chrome/browser/tasks/tab_groups/HnsTabGroupModelFilter";

    public HnsTabGroupModelFilterClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sTabGroupModelFilterClassName, sHnsTabGroupModelFilterClassName);

        deleteField(sTabGroupModelFilterClassName, "mIsResetting");

        changeMethodOwner(
                sTabGroupModelFilterClassName, "getParentId", sHnsTabGroupModelFilterClassName);
    }
}
