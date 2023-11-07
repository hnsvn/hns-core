/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsSearchEnginePreferenceClassAdapter extends HnsClassVisitor {
    static String sSearchEngineSettingsClassName =
            "org/chromium/chrome/browser/search_engines/settings/SearchEngineSettings";

    static String sHnsSearchEnginePreferenceClassName =
            "org/chromium/chrome/browser/search_engines/settings/HnsSearchEnginePreference";

    public HnsSearchEnginePreferenceClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sHnsSearchEnginePreferenceClassName, "mSearchEngineAdapter");
        makeProtectedField(sSearchEngineSettingsClassName, "mSearchEngineAdapter");

        deleteField(sHnsSearchEnginePreferenceClassName, "mProfile");
        makeProtectedField(sSearchEngineSettingsClassName, "mProfile");

        makePublicMethod(sSearchEngineSettingsClassName, "createAdapterIfNecessary");
        addMethodAnnotation(sHnsSearchEnginePreferenceClassName, "createAdapterIfNecessary",
                "Ljava/lang/Override;");
    }
}
