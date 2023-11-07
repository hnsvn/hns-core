/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.preferences.website;

import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.chrome.browser.profiles.Profile;

import java.util.ArrayList;
import java.util.List;

@JNINamespace("chrome::android")
public class HnsShieldsContentSettings {
    static public final String RESOURCE_IDENTIFIER_ADS = "shieldsAds";
    static public final String RESOURCE_IDENTIFIER_TRACKERS = "trackers";
    static public final String RESOURCE_IDENTIFIER_DATA_SAVED = "data_saved";
    static public final String RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES = "httpUpgradableResources";
    static public final String RESOURCE_IDENTIFIER_HNS_SHIELDS = "hnsShields";
    static public final String RESOURCE_IDENTIFIER_FINGERPRINTING = "fingerprinting";
    static public final String RESOURCE_IDENTIFIER_COOKIES = "shieldsCookies";
    static public final String RESOURCE_IDENTIFIER_REFERRERS = "referrers";
    static public final String RESOURCE_IDENTIFIER_JAVASCRIPTS = "javascript";
    static public final String RESOURCE_IDENTIFIER_HTTPS_UPGRADE = "httpsUpgrade";
    static public final String RESOURCE_IDENTIFIER_FORGET_FIRST_PARTY_STORAGE =
            "forgetFirstPartyStorage";

    static public final String BLOCK_RESOURCE = "block";
    static public final String BLOCK_THIRDPARTY_RESOURCE = "block_third_party";
    static public final String DEFAULT = "default";
    static public final String ALLOW_RESOURCE = "allow";
    static public final String AGGRESSIVE = "aggressive";

    public static final int ALWAYS = 0;
    public static final int ASK = 1;
    public static final int NEVER = 2;

    private long mNativeHnsShieldsContentSettings;
    private List<HnsShieldsContentSettingsObserver> mHnsShieldsContentSettingsObservers;
    private static HnsShieldsContentSettings sInstance;

    public static HnsShieldsContentSettings getInstance() {
        if (sInstance == null) sInstance = new HnsShieldsContentSettings();
        return sInstance;
    }

    private HnsShieldsContentSettings() {
        mNativeHnsShieldsContentSettings = 0;
        mHnsShieldsContentSettingsObservers =
            new ArrayList<HnsShieldsContentSettingsObserver>();
        init();
    }

    public void addObserver(HnsShieldsContentSettingsObserver observer) {
        mHnsShieldsContentSettingsObservers.add(observer);
    }

    public void removeObserver(HnsShieldsContentSettingsObserver observer) {
        mHnsShieldsContentSettingsObservers.remove(observer);
    }

    private void init() {
        if (mNativeHnsShieldsContentSettings == 0) {
            HnsShieldsContentSettingsJni.get().init(this);
        }
    }

    @Override
    protected void finalize() {
        destroy();
    }

    private void destroy() {
        if (mNativeHnsShieldsContentSettings == 0) {
            return;
        }
        HnsShieldsContentSettingsJni.get().destroy(mNativeHnsShieldsContentSettings);
    }

    static public void setShields(Profile profile, String host, String resourceIndentifier, boolean value,
                                  boolean fromTopShields) {
        String setting_string = (value ? BLOCK_RESOURCE : ALLOW_RESOURCE);
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_HNS_SHIELDS)) {
            HnsShieldsContentSettingsJni.get().setHnsShieldsEnabled(value, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES)) {
            HnsShieldsContentSettingsJni.get().setHTTPSEverywhereEnabled(value, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_JAVASCRIPTS)) {
            HnsShieldsContentSettingsJni.get().setNoScriptControlType(setting_string, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_FORGET_FIRST_PARTY_STORAGE)) {
            HnsShieldsContentSettingsJni.get().setForgetFirstPartyStorageEnabled(
                    value, host, profile);
        }
    }

    public static void setShieldsValue(Profile profile, String host, String resourceIndentifier,
            String settingOption, boolean fromTopShields) {
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_FINGERPRINTING)) {
            HnsShieldsContentSettingsJni.get().setFingerprintingControlType(
                    settingOption, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_HTTPS_UPGRADE)) {
            HnsShieldsContentSettingsJni.get().setHttpsUpgradeControlType(
                    settingOption, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_COOKIES)) {
            HnsShieldsContentSettingsJni.get().setCookieControlType(settingOption, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_TRACKERS)) {
            HnsShieldsContentSettingsJni.get().setCosmeticFilteringControlType(
                    DEFAULT.equals(settingOption) ? BLOCK_THIRDPARTY_RESOURCE : settingOption, host,
                    profile);
            HnsShieldsContentSettingsJni.get().setAdControlType(
                    BLOCK_THIRDPARTY_RESOURCE.equals(settingOption) ? DEFAULT : settingOption, host,
                    profile);
        }
    }

    public static boolean getShields(Profile profile, String host, String resourceIndentifier) {
        String settings = BLOCK_RESOURCE;
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_HNS_SHIELDS)) {
            return HnsShieldsContentSettingsJni.get().getHnsShieldsEnabled(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES)) {
            return HnsShieldsContentSettingsJni.get().getHTTPSEverywhereEnabled(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_JAVASCRIPTS)) {
            settings = HnsShieldsContentSettingsJni.get().getNoScriptControlType(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_FORGET_FIRST_PARTY_STORAGE)) {
            return HnsShieldsContentSettingsJni.get().getForgetFirstPartyStorageEnabled(
                    host, profile);
        }

        return !settings.equals(ALLOW_RESOURCE);
    }

    public static String getShieldsValue(Profile profile, String host, String resourceIndentifier) {
        String settings = BLOCK_RESOURCE;
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_FINGERPRINTING)) {
            settings = HnsShieldsContentSettingsJni.get().getFingerprintingControlType(
                    host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_HTTPS_UPGRADE)) {
            settings =
                    HnsShieldsContentSettingsJni.get().getHttpsUpgradeControlType(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_COOKIES)) {
            settings = HnsShieldsContentSettingsJni.get().getCookieControlType(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_TRACKERS)) {
            settings = HnsShieldsContentSettingsJni.get().getCosmeticFilteringControlType(
                    host, profile);
            if (settings.equals(BLOCK_THIRDPARTY_RESOURCE)) {
                settings = DEFAULT;
            }
        }
        return settings;
    }

    public static void setHTTPSEverywherePref(boolean value) {
        setShields(Profile.getLastUsedRegularProfile(), "",
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES, value,
                false);
    }

    public static void setFingerprintingPref(String value) {
        setShieldsValue(Profile.getLastUsedRegularProfile(), "",
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING, value, false);
    }

    public static void setHttpsUpgradePref(String value) {
        setShieldsValue(Profile.getLastUsedRegularProfile(), "",
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_HTTPS_UPGRADE, value, false);
    }

    public static void setCookiesPref(String value) {
        setShieldsValue(Profile.getLastUsedRegularProfile(), "",
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_COOKIES, value, false);
    }

    public static void setTrackersPref(String value) {
        setShieldsValue(Profile.getLastUsedRegularProfile(), "",
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_TRACKERS, value, false);
    }

    public static void setJavascriptPref(boolean value) {
        setShields(Profile.getLastUsedRegularProfile(), "",
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_JAVASCRIPTS, value, false);
    }

    public static void setForgetFirstPartyStoragePref(boolean value) {
        setShields(Profile.getLastUsedRegularProfile(), "",
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_FORGET_FIRST_PARTY_STORAGE, value,
                false);
    }

    public static boolean getJavascriptPref() {
        return getShields(Profile.getLastUsedRegularProfile(), "",
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_JAVASCRIPTS);
    }

    public static String getTrackersPref() {
        return getShieldsValue(Profile.getLastUsedRegularProfile(), "",
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_TRACKERS);
    }

    public static String getFingerprintingPref() {
        return getShieldsValue(Profile.getLastUsedRegularProfile(), "",
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING);
    }

    public static String getHttpsUpgradePref() {
        return getShieldsValue(Profile.getLastUsedRegularProfile(), "",
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_HTTPS_UPGRADE);
    }

    public static boolean getHTTPSEverywherePref() {
        return getShields(Profile.getLastUsedRegularProfile(), "",
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES);
    }

    public static boolean getForgetFirstPartyStoragePref() {
        return getShields(Profile.getLastUsedRegularProfile(), "",
                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_FORGET_FIRST_PARTY_STORAGE);
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativeHnsShieldsContentSettings == 0;
        mNativeHnsShieldsContentSettings = nativePtr;
    }

    @CalledByNative
    private void blockedEvent(int tabId, String block_type, String subresource) {
        for (HnsShieldsContentSettingsObserver observer : mHnsShieldsContentSettingsObservers) {
            observer.blockEvent(tabId, block_type, subresource);
        }
    }

    @CalledByNative
    private void savedBandwidth(long savings) {
        for (HnsShieldsContentSettingsObserver observer : mHnsShieldsContentSettingsObservers) {
            observer.savedBandwidth(savings);
        }
    }

    @NativeMethods
    interface Natives {
        void init(HnsShieldsContentSettings self);
        void destroy(long nativeHnsShieldsContentSettings);

        void setHnsShieldsEnabled(boolean enabled, String url, Profile profile);
        boolean getHnsShieldsEnabled(String url, Profile profile);
        void setAdControlType(String type, String url, Profile profile);
        String getAdControlType(String url, Profile profile);
        void setCookieControlType(String type, String url, Profile profile);
        String getCookieControlType(String url, Profile profile);
        void setFingerprintingControlType(String type, String url, Profile profile);
        String getFingerprintingControlType(String url, Profile profile);
        void setHTTPSEverywhereEnabled(boolean enabled, String url, Profile profile);
        boolean getHTTPSEverywhereEnabled(String url, Profile profile);
        void setHttpsUpgradeControlType(String type, String url, Profile profile);
        String getHttpsUpgradeControlType(String url, Profile profile);
        void setNoScriptControlType(String type, String url, Profile profile);
        String getNoScriptControlType(String url, Profile profile);
        void setForgetFirstPartyStorageEnabled(boolean enabled, String url, Profile profile);
        boolean getForgetFirstPartyStorageEnabled(String url, Profile profile);

        void setCosmeticFilteringControlType(String type, String url, Profile profile);
        String getCosmeticFilteringControlType(String url, Profile profile);
    }
}
