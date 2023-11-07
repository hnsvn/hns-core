/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.SharedPreferences;
import android.os.Bundle;

import androidx.preference.Preference;
import androidx.preference.Preference.OnPreferenceChangeListener;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.HnsRelaunchUtils;
import org.chromium.chrome.browser.HnsRewardsNativeWorker;
import org.chromium.chrome.browser.HnsRewardsObserver;
import org.chromium.chrome.browser.preferences.HnsPrefServiceBridge;
import org.chromium.chrome.browser.rewards.HnsRewardsPanel;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;

/**
 * Fragment to keep track of all Hns Rewards related preferences.
 */
public class HnsRewardsPreferences extends HnsPreferenceFragment
        implements OnPreferenceChangeListener, HnsRewardsObserver {
    public static final String PREF_ADS_SWITCH = "ads_switch";

    // flag, if exists: default state (off) for background Hns ads has been set
    public static final String PREF_ADS_SWITCH_DEFAULT_HAS_BEEN_SET = "ads_switch_default_set";

    private ChromeSwitchPreference mAdsSwitch;

    private HnsRewardsNativeWorker mHnsRewardsNativeWorker;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getActivity().setTitle(R.string.hns_ui_hns_rewards);
        SettingsUtils.addPreferencesFromResource(this, R.xml.hns_rewards_preferences);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        mAdsSwitch = (ChromeSwitchPreference) findPreference(PREF_ADS_SWITCH);
        boolean isAdsInBackgroundEnabled = getPrefAdsInBackgroundEnabled();
        mAdsSwitch.setChecked(isAdsInBackgroundEnabled);
        mAdsSwitch.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
            @Override
            public boolean onPreferenceChange(Preference preference, Object newValue) {
                setPrefAdsInBackgroundEnabled((boolean) newValue);
                return true;
            }
        });
    }

    @Override
    public void onStart() {
        mHnsRewardsNativeWorker = HnsRewardsNativeWorker.getInstance();
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

    /**
     * Returns the user preference for whether the hns ads in background is enabled.
     *
     */
    public static boolean getPrefAdsInBackgroundEnabled() {
        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        return sharedPreferences.getBoolean(PREF_ADS_SWITCH, true);
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

    @Override
    public void onDisplayPreferenceDialog(Preference preference) {
        if (preference instanceof HnsRewardsResetPreference) {
            HnsRewardsResetPreferenceDialog dialogFragment =
                    HnsRewardsResetPreferenceDialog.newInstance(
                            (HnsRewardsResetPreference) preference);
            dialogFragment.setTargetFragment(this, 0);
            dialogFragment.show(getFragmentManager(), HnsRewardsResetPreferenceDialog.TAG);
        } else {
            super.onDisplayPreferenceDialog(preference);
        }
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        return true;
    }

    @Override
    public void OnResetTheWholeState(boolean success) {
        if (success) {
            SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
            SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
            sharedPreferencesEditor.putBoolean(
                    HnsRewardsPanel.PREF_GRANTS_NOTIFICATION_RECEIVED, false);
            sharedPreferencesEditor.putBoolean(
                    HnsRewardsPanel.PREF_WAS_HNS_REWARDS_TURNED_ON, false);
            sharedPreferencesEditor.apply();
            HnsPrefServiceBridge.getInstance().setSafetynetCheckFailed(false);
            HnsRelaunchUtils.askForRelaunch(getActivity());
        } else {
            HnsRelaunchUtils.askForRelaunchCustom(getActivity());
        }
    }

    @Override
    public void onCreatePreferences(Bundle bundle, String s) {}
}
