/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsDropdownItemViewInfoListManagerClassAdapter extends HnsClassVisitor {
    static String sDropdownItemViewInfoListManager =
            "org/chromium/chrome/browser/omnibox/suggestions/DropdownItemViewInfoListManager";

    static String sHnsDropdownItemViewInfoListManager =
            "org/chromium/chrome/browser/omnibox/suggestions/HnsDropdownItemViewInfoListManager";

    public HnsDropdownItemViewInfoListManagerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sDropdownItemViewInfoListManager, sHnsDropdownItemViewInfoListManager);
    }
}
