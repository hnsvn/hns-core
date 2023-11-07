/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsLocationBarCoordinatorClassAdapter extends HnsClassVisitor {
    static String sLocationBarCoordinator =
            "org/chromium/chrome/browser/omnibox/LocationBarCoordinator";
    static String sHnsLocationBarCoordinator =
            "org/chromium/chrome/browser/omnibox/HnsLocationBarCoordinator";

    public HnsLocationBarCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sLocationBarCoordinator, sHnsLocationBarCoordinator);

        deleteField(sHnsLocationBarCoordinator, "mLocationBarMediator");
        makeProtectedField(sLocationBarCoordinator, "mLocationBarMediator");

        deleteField(sHnsLocationBarCoordinator, "mUrlBar");
        makeProtectedField(sLocationBarCoordinator, "mUrlBar");
    }
}
