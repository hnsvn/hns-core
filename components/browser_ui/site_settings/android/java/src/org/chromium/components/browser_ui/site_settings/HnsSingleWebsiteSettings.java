/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.components.browser_ui.site_settings;

import android.content.Context;
import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.preference.Preference;

import org.chromium.base.HnsReflectionUtil;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.content_settings.ContentSettingValues;
import org.chromium.components.content_settings.ContentSettingsType;
import org.chromium.content_public.browser.BrowserContextHandle;

public class HnsSingleWebsiteSettings extends SiteSettingsPreferenceFragment {
    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {}

    public static @Nullable String getPreferenceKey(@ContentSettingsType int type) {
        switch (type) {
            case ContentSettingsType.AUTOPLAY:
                return "autoplay_permission_list";
            default:
                return (String) HnsReflectionUtil.InvokeMethod(
                        SingleWebsiteSettings.class, null, "getPreferenceKey", int.class, type);
        }
    }

    public void setupContentSettingsPreferences() {
        Preference preference = new ChromeSwitchPreference(getStyledContext());
        preference.setKey(getPreferenceKey(ContentSettingsType.AUTOPLAY));
        setUpAutoplayPreference(preference);

        // Google Sign In
        preference = new ChromeSwitchPreference(getStyledContext());
        preference.setKey(getPreferenceKey(ContentSettingsType.HNS_GOOGLE_SIGN_IN));
        setUpGoogleSignInPreference(preference);

        // Localhost
        preference = new ChromeSwitchPreference(getStyledContext());
        preference.setKey(getPreferenceKey(ContentSettingsType.HNS_LOCALHOST_ACCESS));
        setUpLocalhostPreference(preference);

        // SingleWebsiteSettings.setupContentSettingsPreferences has its own for loop
        HnsReflectionUtil.InvokeMethod(
                SingleWebsiteSettings.class, this, "setupContentSettingsPreferences");
    }

    private void setUpAutoplayPreference(Preference preference) {
        BrowserContextHandle browserContextHandle =
                getSiteSettingsDelegate().getBrowserContextHandle();
        @Nullable
        Website mSite =
                (Website) HnsReflectionUtil.getField(SingleWebsiteSettings.class, "mSite", this);
        Integer currentValue =
                mSite.getContentSetting(browserContextHandle, ContentSettingsType.AUTOPLAY);
        // In order to always show the autoplay permission, set it up with the default value if it
        // doesn't have a current value.
        if (currentValue == null) {
            currentValue = WebsitePreferenceBridge.isCategoryEnabled(
                                   browserContextHandle, ContentSettingsType.AUTOPLAY)
                    ? ContentSettingValues.ALLOW
                    : ContentSettingValues.BLOCK;
        }
        // Not possible to embargo AUTOPLAY.
        HnsReflectionUtil.InvokeMethod(SingleWebsiteSettings.class, this,
                "setupContentSettingsPreference", Preference.class, preference, Integer.class,
                currentValue, boolean.class, false);
    }

    private void setUpGoogleSignInPreference(Preference preference) {
        BrowserContextHandle browserContextHandle =
                getSiteSettingsDelegate().getBrowserContextHandle();
        @Nullable
        Website mSite =
                (Website) HnsReflectionUtil.getField(SingleWebsiteSettings.class, "mSite", this);
        Integer currentValue = mSite.getContentSetting(
                browserContextHandle, ContentSettingsType.HNS_GOOGLE_SIGN_IN);
        if (currentValue == null) {
            currentValue = WebsitePreferenceBridge.isCategoryEnabled(
                                   browserContextHandle, ContentSettingsType.HNS_GOOGLE_SIGN_IN)
                    ? ContentSettingValues.ASK
                    : ContentSettingValues.BLOCK;
        }
        HnsReflectionUtil.InvokeMethod(SingleWebsiteSettings.class, this,
                "setupContentSettingsPreference", Preference.class, preference, Integer.class,
                currentValue, boolean.class, false);
    }

    private void setUpLocalhostPreference(Preference preference) {
        BrowserContextHandle browserContextHandle =
                getSiteSettingsDelegate().getBrowserContextHandle();
        @Nullable
        Website mSite =
                (Website) HnsReflectionUtil.getField(SingleWebsiteSettings.class, "mSite", this);
        Integer currentValue = mSite.getContentSetting(
                browserContextHandle, ContentSettingsType.HNS_LOCALHOST_ACCESS);
        if (currentValue == null) {
            currentValue = WebsitePreferenceBridge.isCategoryEnabled(
                                   browserContextHandle, ContentSettingsType.HNS_LOCALHOST_ACCESS)
                    ? ContentSettingValues.ASK
                    : ContentSettingValues.BLOCK;
        }
        HnsReflectionUtil.InvokeMethod(SingleWebsiteSettings.class, this,
                "setupContentSettingsPreference", Preference.class, preference, Integer.class,
                currentValue, boolean.class, false);
    }

    private Context getStyledContext() {
        return getPreferenceManager().getContext();
    }
}
