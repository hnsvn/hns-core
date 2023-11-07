/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsMostVisitedTilesMediatorClassAdapter extends HnsClassVisitor {
    static String sMostVisitedTilesMediatorClassName =
            "org/chromium/chrome/browser/suggestions/tile/MostVisitedTilesMediator";
    static String sHnsMostVisitedTilesMediatorClassName =
            "org/chromium/chrome/browser/suggestions/tile/HnsMostVisitedTilesMediator";

    public HnsMostVisitedTilesMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sMostVisitedTilesMediatorClassName, sHnsMostVisitedTilesMediatorClassName);

        makePublicMethod(sMostVisitedTilesMediatorClassName, "updateTilePlaceholderVisibility");
        addMethodAnnotation(sHnsMostVisitedTilesMediatorClassName,
                "updateTilePlaceholderVisibility", "Ljava/lang/Override;");

        deleteField(sHnsMostVisitedTilesMediatorClassName, "mTileGroup");
        makeProtectedField(sMostVisitedTilesMediatorClassName, "mTileGroup");
    }
}
