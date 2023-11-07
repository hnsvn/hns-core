/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsDropdownItemViewInfoListBuilderClassAdapter extends HnsClassVisitor {
    static String sDropdownItemViewInfoListBuilder =
            "org/chromium/chrome/browser/omnibox/suggestions/DropdownItemViewInfoListBuilder";

    static String sHnsDropdownItemViewInfoListBuilder =
            "org/chromium/chrome/browser/omnibox/suggestions/HnsDropdownItemViewInfoListBuilder";

    public HnsDropdownItemViewInfoListBuilderClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sDropdownItemViewInfoListBuilder, sHnsDropdownItemViewInfoListBuilder);
    }
}
