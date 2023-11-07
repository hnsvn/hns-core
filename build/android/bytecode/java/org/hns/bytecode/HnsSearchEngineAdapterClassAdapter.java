/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsSearchEngineAdapterClassAdapter extends HnsClassVisitor {
    static String sSearchEngineAdapterClassName =
            "org/chromium/chrome/browser/search_engines/settings/SearchEngineAdapter";

    static String sHnsSearchEngineAdapterClassName =
            "org/chromium/chrome/browser/search_engines/settings/HnsSearchEngineAdapter";

    static String sHnsBaseSearchEngineAdapterClassName =
            "org/chromium/chrome/browser/search_engines/settings/HnsBaseSearchEngineAdapter";

    static String sSearchEngineSettingsClassName =
            "org/chromium/chrome/browser/search_engines/settings/SearchEngineSettings";

    static String sHnsSearchEnginePreferenceClassName =
            "org/chromium/chrome/browser/search_engines/settings/HnsSearchEnginePreference";

    static String sMethodGetSearchEngineSourceType = "getSearchEngineSourceType";

    static String sMethodSortAndFilterUnnecessaryTemplateUrl =
            "sortAndFilterUnnecessaryTemplateUrl";

    public HnsSearchEngineAdapterClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sSearchEngineAdapterClassName, sHnsBaseSearchEngineAdapterClassName);

        changeMethodOwner(sSearchEngineAdapterClassName, sMethodGetSearchEngineSourceType,
                sHnsBaseSearchEngineAdapterClassName);

        changeMethodOwner(sSearchEngineAdapterClassName, sMethodSortAndFilterUnnecessaryTemplateUrl,
                sHnsBaseSearchEngineAdapterClassName);

        deleteField(sHnsSearchEngineAdapterClassName, "mProfile");
        makeProtectedField(sSearchEngineAdapterClassName, "mProfile");
    }
}
