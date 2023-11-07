/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import static org.chromium.chrome.browser.settings.MainSettings.PREF_UI_THEME;

import android.content.SharedPreferences;
import android.os.Bundle;

import androidx.preference.Preference;

import org.chromium.base.HnsFeatureList;
import org.chromium.base.HnsPreferenceKeys;
import org.chromium.base.ContextUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.HnsFeatureUtil;
import org.chromium.chrome.browser.HnsRelaunchUtils;
import org.chromium.chrome.browser.HnsRewardsNativeWorker;
import org.chromium.chrome.browser.HnsRewardsObserver;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.night_mode.NightModeUtils;
import org.chromium.chrome.browser.preferences.HnsPref;
import org.chromium.chrome.browser.preferences.SharedPreferencesManager;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.tasks.tab_management.HnsTabUiFeatureUtilities;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarConfiguration;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.ui.base.DeviceFormFactor;

public class AppearancePreferences extends HnsPreferenceFragment
        implements Preference.OnPreferenceChangeListener, HnsRewardsObserver {
    public static final String PREF_HIDE_HNS_REWARDS_ICON = "hide_hns_rewards_icon";
    public static final String PREF_HIDE_HNS_REWARDS_ICON_MIGRATION =
            "hide_hns_rewards_icon_migration";
    public static final String PREF_SHOW_HNS_REWARDS_ICON = "show_hns_rewards_icon";
    public static final String PREF_ADS_SWITCH = "ads_switch";
    public static final String PREF_HNS_NIGHT_MODE_ENABLED = "hns_night_mode_enabled_key";
    public static final String PREF_HNS_DISABLE_SHARING_HUB = "hns_disable_sharing_hub";
    public static final String PREF_HNS_ENABLE_TAB_GROUPS = "hns_enable_tab_groups";
    public static final String PREF_HNS_ENABLE_SPEEDREADER = "hns_enable_speedreader";

    private HnsRewardsNativeWorker mHnsRewardsNativeWorker;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getActivity().setTitle(R.string.prefs_appearance);
        SettingsUtils.addPreferencesFromResource(this, R.xml.appearance_preferences);
        boolean isTablet = DeviceFormFactor.isNonMultiDisplayContextOnTablet(
                ContextUtils.getApplicationContext());
        if (isTablet) {
            removePreferenceIfPresent(HnsPreferenceKeys.HNS_BOTTOM_TOOLBAR_ENABLED_KEY);
        }

        if (!NightModeUtils.isNightModeSupported()) {
            removePreferenceIfPresent(PREF_UI_THEME);
        }

        mHnsRewardsNativeWorker = HnsRewardsNativeWorker.getInstance();
        if (mHnsRewardsNativeWorker == null || !mHnsRewardsNativeWorker.IsSupported()) {
            removePreferenceIfPresent(PREF_SHOW_HNS_REWARDS_ICON);
        }

        if (!ChromeFeatureList.isEnabled(HnsFeatureList.HNS_SPEEDREADER)) {
            removePreferenceIfPresent(PREF_HNS_ENABLE_SPEEDREADER);
        }
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {}

    private void removePreferenceIfPresent(String key) {
        Preference preference = getPreferenceScreen().findPreference(key);
        if (preference != null) getPreferenceScreen().removePreference(preference);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        ChromeSwitchPreference showHnsRewardsIconPref =
                (ChromeSwitchPreference) findPreference(PREF_SHOW_HNS_REWARDS_ICON);
        if (showHnsRewardsIconPref != null) {
            SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
            showHnsRewardsIconPref.setChecked(
                    sharedPreferences.getBoolean(PREF_SHOW_HNS_REWARDS_ICON, true));
            showHnsRewardsIconPref.setOnPreferenceChangeListener(this);
        }

        ChromeSwitchPreference adsSwitchPref =
                (ChromeSwitchPreference) findPreference(PREF_ADS_SWITCH);
        if (adsSwitchPref != null) {
            adsSwitchPref.setChecked(getPrefAdsInBackgroundEnabled());
            adsSwitchPref.setOnPreferenceChangeListener(this);
        }

        Preference nightModeEnabled = findPreference(PREF_HNS_NIGHT_MODE_ENABLED);
        nightModeEnabled.setOnPreferenceChangeListener(this);
        if (nightModeEnabled instanceof ChromeSwitchPreference) {
            ((ChromeSwitchPreference) nightModeEnabled)
                    .setChecked(ChromeFeatureList.isEnabled(
                            HnsFeatureList.FORCE_WEB_CONTENTS_DARK_MODE));
        }

        Preference enableBottomToolbar =
                findPreference(HnsPreferenceKeys.HNS_BOTTOM_TOOLBAR_ENABLED_KEY);
        if (enableBottomToolbar != null) {
            enableBottomToolbar.setOnPreferenceChangeListener(this);
            if (enableBottomToolbar instanceof ChromeSwitchPreference) {
                boolean isTablet = DeviceFormFactor.isNonMultiDisplayContextOnTablet(
                        ContextUtils.getApplicationContext());
                ((ChromeSwitchPreference) enableBottomToolbar)
                        .setChecked(
                                !isTablet && BottomToolbarConfiguration.isBottomToolbarEnabled());
            }
        }

        Preference disableSharingHub = findPreference(PREF_HNS_DISABLE_SHARING_HUB);
        if (disableSharingHub != null) {
            disableSharingHub.setOnPreferenceChangeListener(this);
            if (disableSharingHub instanceof ChromeSwitchPreference) {
                ((ChromeSwitchPreference) disableSharingHub)
                        .setChecked(SharedPreferencesManager.getInstance().readBoolean(
                                HnsPreferenceKeys.HNS_DISABLE_SHARING_HUB, false));
            }
        }

        Preference enableTabGroups = findPreference(PREF_HNS_ENABLE_TAB_GROUPS);
        if (enableTabGroups != null) {
            enableTabGroups.setOnPreferenceChangeListener(this);
            if (enableTabGroups instanceof ChromeSwitchPreference) {
                ((ChromeSwitchPreference) enableTabGroups)
                        .setChecked(HnsTabUiFeatureUtilities.isHnsTabGroupsEnabled());
            }
        }

        Preference enableSpeedreader = findPreference(PREF_HNS_ENABLE_SPEEDREADER);
        if (enableSpeedreader != null) {
            enableSpeedreader.setOnPreferenceChangeListener(this);
            if (enableSpeedreader instanceof ChromeSwitchPreference) {
                ((ChromeSwitchPreference) enableSpeedreader)
                        .setChecked(UserPrefs.get(Profile.getLastUsedRegularProfile())
                                            .getBoolean(HnsPref.SPEEDREADER_PREF_ENABLED));
            }
        }
    }

    @Override
    public void onStart() {
        if (mHnsRewardsNativeWorker != null) {
            mHnsRewardsNativeWorker.AddObserver(this);
        }
        super.onStart();
    }

    @Override
    public void onStop() {
        if (mHnsRewardsNativeWorker != null) {
            mHnsRewardsNativeWorker.RemoveObserver(this);
        }
        super.onStop();
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        String key = preference.getKey();
        boolean shouldRelaunch = false;
        if (HnsPreferenceKeys.HNS_BOTTOM_TOOLBAR_ENABLED_KEY.equals(key)) {
            SharedPreferences prefs = ContextUtils.getAppSharedPreferences();
            Boolean originalStatus = BottomToolbarConfiguration.isBottomToolbarEnabled();
            prefs.edit()
                    .putBoolean(
                            HnsPreferenceKeys.HNS_BOTTOM_TOOLBAR_ENABLED_KEY, !originalStatus)
                    .apply();
            shouldRelaunch = true;
        } else if (PREF_SHOW_HNS_REWARDS_ICON.equals(key)) {
            SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
            SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
            sharedPreferencesEditor.putBoolean(PREF_SHOW_HNS_REWARDS_ICON, !(boolean) newValue);
            sharedPreferencesEditor.apply();
            shouldRelaunch = true;
        } else if (PREF_ADS_SWITCH.equals(key)) {
            setPrefAdsInBackgroundEnabled((boolean) newValue);
        } else if (PREF_HNS_NIGHT_MODE_ENABLED.equals(key)) {
            HnsFeatureUtil.enableFeature(
                    HnsFeatureList.ENABLE_FORCE_DARK, (boolean) newValue, true);
            shouldRelaunch = true;
        } else if (PREF_HNS_DISABLE_SHARING_HUB.equals(key)) {
            SharedPreferencesManager.getInstance().writeBoolean(
                    HnsPreferenceKeys.HNS_DISABLE_SHARING_HUB, (boolean) newValue);
        } else if (PREF_HNS_ENABLE_TAB_GROUPS.equals(key)) {
            SharedPreferencesManager.getInstance().writeBoolean(
                    HnsPreferenceKeys.HNS_TAB_GROUPS_ENABLED, (boolean) newValue);
        } else if (PREF_HNS_ENABLE_SPEEDREADER.equals(key)) {
            UserPrefs.get(Profile.getLastUsedRegularProfile())
                    .setBoolean(HnsPref.SPEEDREADER_PREF_ENABLED, (boolean) newValue);
            shouldRelaunch = true;
        }
        if (shouldRelaunch) {
            HnsRelaunchUtils.askForRelaunch(getActivity());
        }

        return true;
    }

    /**
     * Returns the user preference for whether the hns ads in background is enabled.
     *
     */
    public static boolean getPrefAdsInBackgroundEnabled() {
        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        return sharedPreferences.getBoolean(PREF_ADS_SWITCH, false);
    }

    /**
     * Sets the user preference for whether the hns ads in background is enabled.
     */
    public void setPrefAdsInBackgroundEnabled(boolean enabled) {
        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREF_ADS_SWITCH, enabled);
        sharedPreferencesEditor.apply();
    }
}
