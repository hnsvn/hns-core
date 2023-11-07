/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsAutocompleteCoordinatorClassAdapter extends HnsClassVisitor {
    static String sAutocompleteCoordinator =
            "org/chromium/chrome/browser/omnibox/suggestions/AutocompleteCoordinator";

    static String sHnsAutocompleteCoordinator =
            "org/chromium/chrome/browser/omnibox/suggestions/HnsAutocompleteCoordinator";

    public HnsAutocompleteCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sAutocompleteCoordinator, sHnsAutocompleteCoordinator);
        changeMethodOwner(
                sAutocompleteCoordinator, "createViewProvider", sHnsAutocompleteCoordinator);
    }
}
