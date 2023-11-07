/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsManageSyncSettingsClassAdapter extends HnsClassVisitor {
    static String sManageSyncSettingsClassName =
            "org/chromium/chrome/browser/sync/settings/ManageSyncSettings";
    static String sHnsManageSyncSettingsClassName =
            "org/chromium/chrome/browser/sync/settings/HnsManageSyncSettings";

    HnsManageSyncSettingsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sHnsManageSyncSettingsClassName, "mGoogleActivityControls");
        makeProtectedField(sManageSyncSettingsClassName, "mGoogleActivityControls");

        deleteField(sHnsManageSyncSettingsClassName, "mSyncEncryption");
        makeProtectedField(sManageSyncSettingsClassName, "mSyncEncryption");
    }
}
