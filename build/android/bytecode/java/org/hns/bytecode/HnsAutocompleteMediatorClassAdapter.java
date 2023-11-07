/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsAutocompleteMediatorClassAdapter extends HnsClassVisitor {
    static String sAutocompleteMediator =
            "org/chromium/chrome/browser/omnibox/suggestions/AutocompleteMediator";
    static String sHnsAutocompleteMediator =
            "org/chromium/chrome/browser/omnibox/suggestions/HnsAutocompleteMediator";

    public HnsAutocompleteMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sAutocompleteMediator, sHnsAutocompleteMediator);

        deleteField(sHnsAutocompleteMediator, "mNativeInitialized");
        makeProtectedField(sAutocompleteMediator, "mNativeInitialized");

        deleteField(sHnsAutocompleteMediator, "mDropdownViewInfoListManager");
        makeProtectedField(sAutocompleteMediator, "mDropdownViewInfoListManager");
    }
}
