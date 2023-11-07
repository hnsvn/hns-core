/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.components.browser_ui.site_settings;

import org.chromium.components.browser_ui.site_settings.WebsitePermissionsFetcher.WebsitePermissionsType;
import org.chromium.components.content_settings.ContentSettingsType;

public class HnsWebsitePermissionsFetcher {
    public static WebsitePermissionsType getPermissionsType(
            @ContentSettingsType int contentSettingsType) {
        switch (contentSettingsType) {
            case ContentSettingsType.AUTOPLAY:
            case ContentSettingsType.HNS_GOOGLE_SIGN_IN:
            case ContentSettingsType.HNS_LOCALHOST_ACCESS:
                return WebsitePermissionsType.CONTENT_SETTING_EXCEPTION;
        }
        return WebsitePermissionsFetcher.getPermissionsType(contentSettingsType);
    }
}
