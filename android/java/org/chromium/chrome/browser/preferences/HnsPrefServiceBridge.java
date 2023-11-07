/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.preferences;

import org.chromium.base.ThreadUtils;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.chrome.browser.profiles.Profile;

/**
 * Please don't add anything in that file. We are going to refactor it soon.
 * Check this PRs on how to handle preferences correctly:
 * https://github.com/hnsvn/hns-core/pull/16356
 * https://github.com/hnsvn/hns-core/pull/15905
 * For the local_state based prefs please look on the PR:
 * https://github.com/hnsvn/hns-core/pull/16486
 * Contact code owners if you have additional questions.
 */

@JNINamespace("chrome::android")
public class HnsPrefServiceBridge {
    private HnsPrefServiceBridge() {
    }

    private static HnsPrefServiceBridge sInstance;

    public static HnsPrefServiceBridge getInstance() {
        ThreadUtils.assertOnUiThread();
        if (sInstance == null) {
            sInstance = new HnsPrefServiceBridge();
        }
        return sInstance;
    }

    public void setCookiesBlockType(String type) {
        HnsPrefServiceBridgeJni.get().setCookiesBlockType(type);
    }

    public String getCookiesBlockType() {
        return HnsPrefServiceBridgeJni.get().getCookiesBlockType();
    }

    public void setPlayYTVideoInBrowserEnabled(boolean enabled) {
        HnsPrefServiceBridgeJni.get().setPlayYTVideoInBrowserEnabled(enabled);
    }

    public boolean getPlayYTVideoInBrowserEnabled() {
        return HnsPrefServiceBridgeJni.get().getPlayYTVideoInBrowserEnabled();
    }

    public void setDesktopModeEnabled(boolean enabled) {
        HnsPrefServiceBridgeJni.get().setDesktopModeEnabled(enabled);
    }

    public boolean getDesktopModeEnabled() {
        return HnsPrefServiceBridgeJni.get().getDesktopModeEnabled();
    }

    public void setBackgroundVideoPlaybackEnabled(boolean enabled) {
        HnsPrefServiceBridgeJni.get().setBackgroundVideoPlaybackEnabled(enabled);
    }

    public boolean getBackgroundVideoPlaybackEnabled() {
        return HnsPrefServiceBridgeJni.get().getBackgroundVideoPlaybackEnabled();
    }

    public long getTrackersBlockedCount(Profile profile) {
        return HnsPrefServiceBridgeJni.get().getTrackersBlockedCount(profile);
    }

    public long getAdsBlockedCount(Profile profile) {
        return HnsPrefServiceBridgeJni.get().getAdsBlockedCount(profile);
    }

    public long getDataSaved(Profile profile) {
        return HnsPrefServiceBridgeJni.get().getDataSaved(profile);
    }

    /**
     * @param whether SafetyNet check failed.
     */
    public void setSafetynetCheckFailed(boolean value) {
        HnsPrefServiceBridgeJni.get().setSafetynetCheckFailed(value);
    }

    public boolean getSafetynetCheckFailed() {
        return HnsPrefServiceBridgeJni.get().getSafetynetCheckFailed();
    }

    public void resetPromotionLastFetchStamp() {
        HnsPrefServiceBridgeJni.get().resetPromotionLastFetchStamp();
    }

    public void setOldTrackersBlockedCount(Profile profile, long count) {
        HnsPrefServiceBridgeJni.get().setOldTrackersBlockedCount(profile, count);
    }

    public void setOldAdsBlockedCount(Profile profile, long count) {
        HnsPrefServiceBridgeJni.get().setOldAdsBlockedCount(profile, count);
    }

    public void setOldHttpsUpgradesCount(Profile profile, long count) {
        HnsPrefServiceBridgeJni.get().setOldHttpsUpgradesCount(profile, count);
    }

    public boolean GetBooleanForContentSetting(int content_type) {
        return HnsPrefServiceBridgeJni.get().getBooleanForContentSetting(content_type);
    }

    public void setWebrtcPolicy(int policy) {
        HnsPrefServiceBridgeJni.get().setWebrtcPolicy(policy);
    }

    public int getWebrtcPolicy() {
        return HnsPrefServiceBridgeJni.get().getWebrtcPolicy();
    }

    public void setNewsOptIn(boolean value) {
        HnsPrefServiceBridgeJni.get().setNewsOptIn(value);
    }

    public boolean getNewsOptIn() {
        boolean getNewsOptIn = HnsPrefServiceBridgeJni.get().getNewsOptIn();
        return HnsPrefServiceBridgeJni.get().getNewsOptIn();
    }
    public void setShowNews(boolean value) {
        HnsPrefServiceBridgeJni.get().setShowNews(value);
    }

    public boolean getShowNews() {
        return HnsPrefServiceBridgeJni.get().getShowNews();
    }

    @NativeMethods
    interface Natives {
        void setCookiesBlockType(String type);
        String getCookiesBlockType();

        void setPlayYTVideoInBrowserEnabled(boolean enabled);
        boolean getPlayYTVideoInBrowserEnabled();

        void setDesktopModeEnabled(boolean enabled);
        boolean getDesktopModeEnabled();

        void setBackgroundVideoPlaybackEnabled(boolean enabled);
        boolean getBackgroundVideoPlaybackEnabled();

        long getTrackersBlockedCount(Profile profile);
        long getAdsBlockedCount(Profile profile);
        long getDataSaved(Profile profile);

        // Used to pass total stat from upgrading old tabs based browser
        // to a new core based
        void setOldTrackersBlockedCount(Profile profile, long count);
        void setOldAdsBlockedCount(Profile profile, long count);
        void setOldHttpsUpgradesCount(Profile profile, long count);

        void setSafetynetCheckFailed(boolean value);
        boolean getSafetynetCheckFailed();

        void resetPromotionLastFetchStamp();
        boolean getBooleanForContentSetting(int content_type);

        void setWebrtcPolicy(int policy);
        int getWebrtcPolicy();

        void setNewsOptIn(boolean value);
        boolean getNewsOptIn();

        void setShowNews(boolean value);
        boolean getShowNews();
    }
}
