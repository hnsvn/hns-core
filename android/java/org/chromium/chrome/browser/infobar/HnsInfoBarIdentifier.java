/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.infobar;

import androidx.annotation.IntDef;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

// Reflects enum items from chromium_src/components/infobars/core/infobar_delegate.h

@IntDef({HnsInfoBarIdentifier.INVALID, HnsInfoBarIdentifier.HNS_CONFIRM_P3A_INFOBAR_DELEGATE,
        HnsInfoBarIdentifier.WAYBACK_MACHINE_INFOBAR_DELEGATE,
        HnsInfoBarIdentifier.SYNC_V2_MIGRATE_INFOBAR_DELEGATE,
        HnsInfoBarIdentifier.ANDROID_SYSTEM_SYNC_DISABLED_INFOBAR,
        HnsInfoBarIdentifier.SYNC_CANNOT_RUN_INFOBAR,
        HnsInfoBarIdentifier.WEB_DISCOVERY_INFOBAR_DELEGATE,
        HnsInfoBarIdentifier.HNS_SYNC_ACCOUNT_DELETED_INFOBAR})

@Retention(RetentionPolicy.SOURCE)
public @interface HnsInfoBarIdentifier {
    int INVALID = -1;
    int HNS_CONFIRM_P3A_INFOBAR_DELEGATE = 500;
    int WAYBACK_MACHINE_INFOBAR_DELEGATE = 502;
    int SYNC_V2_MIGRATE_INFOBAR_DELEGATE = 503;
    int ANDROID_SYSTEM_SYNC_DISABLED_INFOBAR = 504;
    int SYNC_CANNOT_RUN_INFOBAR = 505;
    int WEB_DISCOVERY_INFOBAR_DELEGATE = 506;
    int HNS_SYNC_ACCOUNT_DELETED_INFOBAR = 507;
}
