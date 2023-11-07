/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsSingleCategorySettingsClassAdapter extends HnsClassVisitor {
    static String sSingleCategorySettingsClassName = "org/chromium/components/browser_ui/site_settings/SingleCategorySettings";
    static String sHnsSingleCategorySettingsClassName = "org/chromium/components/browser_ui/site_settings/HnsSingleCategorySettings";

    public HnsSingleCategorySettingsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sSingleCategorySettingsClassName, sHnsSingleCategorySettingsClassName);
        changeMethodOwner(sSingleCategorySettingsClassName, "onOptionsItemSelected",
                sHnsSingleCategorySettingsClassName);
        changeMethodOwner(sSingleCategorySettingsClassName, "getAddExceptionDialogMessage",
                sHnsSingleCategorySettingsClassName);
        changeMethodOwner(sSingleCategorySettingsClassName, "resetList",
                sHnsSingleCategorySettingsClassName);
    }
}
