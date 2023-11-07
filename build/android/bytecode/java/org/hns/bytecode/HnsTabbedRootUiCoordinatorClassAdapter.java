/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsTabbedRootUiCoordinatorClassAdapter extends HnsClassVisitor {
    static String sTabbedRootUiCoordinatorClassName =
            "org/chromium/chrome/browser/tabbed_mode/TabbedRootUiCoordinator";
    static String sHnsTabbedRootUiCoordinatorClassName =
            "org/chromium/chrome/browser/tabbed_mode/HnsTabbedRootUiCoordinator";

    public HnsTabbedRootUiCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sTabbedRootUiCoordinatorClassName, sHnsTabbedRootUiCoordinatorClassName);
    }
}
