/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.components.browser_ui.site_settings;

import org.chromium.base.HnsReflectionUtil;
import org.chromium.build.annotations.UsedByReflection;
import org.chromium.components.content_settings.ContentSettingValues;
import org.chromium.components.content_settings.ContentSettingsType;
import org.chromium.content_public.browser.BrowserContextHandle;

@UsedByReflection("Website")
public class HnsWebsite {
    public void setContentSetting(BrowserContextHandle browserContextHandle,
            @ContentSettingsType int type, @ContentSettingValues int value) {
        PermissionInfo info = (PermissionInfo) HnsReflectionUtil.InvokeMethod(
                Website.class, this, "getPermissionInfo", int.class, type);
        if (info != null) {
            info.setContentSetting(browserContextHandle, value);
            return;
        }

        ContentSettingException exception =
                (ContentSettingException) HnsReflectionUtil.InvokeMethod(
                        Website.class, this, "getContentSettingException", int.class, type);
        if (type == ContentSettingsType.AUTOPLAY) {
            if (exception == null) {
                exception = new ContentSettingException(ContentSettingsType.AUTOPLAY,
                        ((WebsiteAddress) HnsReflectionUtil.InvokeMethod(
                                 Website.class, this, "getAddress"))
                                .getHost(),
                        value, "", false);
                HnsReflectionUtil.InvokeMethod(Website.class, this, "setContentSettingException",
                        int.class, type, ContentSettingException.class, exception);
            }
        } else if (type == ContentSettingsType.HNS_GOOGLE_SIGN_IN) {
            if (exception == null) {
                exception = new ContentSettingException(ContentSettingsType.HNS_GOOGLE_SIGN_IN,
                        ((WebsiteAddress) HnsReflectionUtil.InvokeMethod(
                                 Website.class, this, "getAddress"))
                                .getHost(),
                        value, "", false);
                HnsReflectionUtil.InvokeMethod(Website.class, this, "setContentSettingException",
                        int.class, type, ContentSettingException.class, exception);
            }
        } else if (type == ContentSettingsType.HNS_LOCALHOST_ACCESS) {
            if (exception == null) {
                exception = new ContentSettingException(ContentSettingsType.HNS_LOCALHOST_ACCESS,
                        ((WebsiteAddress) HnsReflectionUtil.InvokeMethod(
                                 Website.class, this, "getAddress"))
                                .getHost(),
                        value, "", false);
                HnsReflectionUtil.InvokeMethod(Website.class, this, "setContentSettingException",
                        int.class, type, ContentSettingException.class, exception);
            }
        }

        HnsReflectionUtil.InvokeMethod(Website.class, this, "setContentSetting",
                BrowserContextHandle.class, browserContextHandle, int.class, type, int.class,
                value);
    }
}
