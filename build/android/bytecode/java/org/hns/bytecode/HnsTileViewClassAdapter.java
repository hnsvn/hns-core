/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsTileViewClassAdapter extends HnsClassVisitor {
    static String sSuggestionsTileViewClassName =
            "org/chromium/chrome/browser/suggestions/tile/SuggestionsTileView";

    static String sHnsSuggestionsTileViewBaseClassName =
            "org/chromium/chrome/browser/suggestions/tile/HnsTileView";

    public HnsTileViewClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sSuggestionsTileViewClassName, sHnsSuggestionsTileViewBaseClassName);
    }
}
