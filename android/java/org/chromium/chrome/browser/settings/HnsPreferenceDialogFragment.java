/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

import androidx.appcompat.app.AlertDialog;
import androidx.preference.Preference;
import androidx.preference.PreferenceDialogFragmentCompat;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.preferences.website.HnsShieldsContentSettings;
import org.chromium.chrome.browser.privacy.settings.HnsPrivacySettings;
import org.chromium.chrome.browser.profiles.Profile;

public class HnsPreferenceDialogFragment extends PreferenceDialogFragmentCompat {
    public static final String TAG = "HnsPreferenceDialogFragment";
    private RadioGroup mRadioGroup;
    private CharSequence[] mDialogEntries;
    private HnsDialogPreference dialogPreference;
    private Preference.OnPreferenceChangeListener onPreferenceChangeListener;
    private int newValue;

    private static String currentPreference;

    public void setPreferenceDialogListener(Preference.OnPreferenceChangeListener listener) {
        this.onPreferenceChangeListener = listener;
    }

    public static HnsPreferenceDialogFragment newInstance(HnsDialogPreference preference) {
        HnsPreferenceDialogFragment fragment = new HnsPreferenceDialogFragment();
        Bundle bundle = new Bundle(1);
        bundle.putString(PreferenceDialogFragmentCompat.ARG_KEY, preference.getKey());
        currentPreference = preference.getKey();
        fragment.setArguments(bundle);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        dialogPreference = (HnsDialogPreference) getPreference();
    }

    @Override
    public void onDialogClosed(boolean positiveResult) {
        if (onPreferenceChangeListener != null) {
            SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
            if (currentPreference.equals(HnsPrivacySettings.PREF_FINGERPRINTING_PROTECTION)) {
                onPreferenceChangeListener.onPreferenceChange(dialogPreference,
                        HnsShieldsContentSettings.getShieldsValue(
                                Profile.getLastUsedRegularProfile(), "",
                                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING));
            } else if (currentPreference.equals(HnsPrivacySettings.PREF_BLOCK_TRACKERS_ADS)) {
                onPreferenceChangeListener.onPreferenceChange(dialogPreference,
                        HnsShieldsContentSettings.getShieldsValue(
                                Profile.getLastUsedRegularProfile(), "",
                                HnsShieldsContentSettings.RESOURCE_IDENTIFIER_TRACKERS));
            } else {
                onPreferenceChangeListener.onPreferenceChange(
                        dialogPreference, sharedPreferences.getInt(currentPreference, 1));
            }
        }
    }

    @Override
    protected void onPrepareDialogBuilder(AlertDialog.Builder builder) {
        super.onPrepareDialogBuilder(builder);
        builder.setCancelable(false);
        if (mRadioGroup != null) {
            mRadioGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
                @Override
                public void onCheckedChanged(RadioGroup group, int checkedId) {
                    newValue = checkedId;
                    SharedPreferences.Editor sharedPreferencesEditor =
                            ContextUtils.getAppSharedPreferences().edit();
                    if (currentPreference.equals(
                                HnsPrivacySettings.PREF_FINGERPRINTING_PROTECTION)) {
                        if ((int) newValue == 0) {
                            HnsShieldsContentSettings.setShieldsValue(
                                    Profile.getLastUsedRegularProfile(), "",
                                    HnsShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING,
                                    HnsShieldsContentSettings.BLOCK_RESOURCE, false);
                        } else if ((int) newValue == 1) {
                            HnsShieldsContentSettings.setShieldsValue(
                                    Profile.getLastUsedRegularProfile(), "",
                                    HnsShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING,
                                    HnsShieldsContentSettings.DEFAULT, false);
                        } else {
                            HnsShieldsContentSettings.setShieldsValue(
                                    Profile.getLastUsedRegularProfile(), "",
                                    HnsShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING,
                                    HnsShieldsContentSettings.ALLOW_RESOURCE, false);
                        }
                    } else if (currentPreference.equals(
                                       HnsPrivacySettings.PREF_BLOCK_TRACKERS_ADS)) {
                        switch ((int) newValue) {
                            case 0:
                                HnsShieldsContentSettings.setShieldsValue(
                                        Profile.getLastUsedRegularProfile(), "",
                                        HnsShieldsContentSettings.RESOURCE_IDENTIFIER_TRACKERS,
                                        HnsShieldsContentSettings.BLOCK_RESOURCE, false);
                                break;
                            case 1:
                                HnsShieldsContentSettings.setShieldsValue(
                                        Profile.getLastUsedRegularProfile(), "",
                                        HnsShieldsContentSettings.RESOURCE_IDENTIFIER_TRACKERS,
                                        HnsShieldsContentSettings.DEFAULT, false);
                                break;
                            default:
                                HnsShieldsContentSettings.setShieldsValue(
                                        Profile.getLastUsedRegularProfile(), "",
                                        HnsShieldsContentSettings.RESOURCE_IDENTIFIER_TRACKERS,
                                        HnsShieldsContentSettings.ALLOW_RESOURCE, false);
                                break;
                        }
                    } else {
                        sharedPreferencesEditor.putInt(currentPreference, (int) newValue);
                    }

                    sharedPreferencesEditor.apply();
                }
            });
        }

        builder.setPositiveButton(null, null);
        builder.setNegativeButton(getResources().getString(R.string.hns_sync_btn_done), null);
    }

    @Override
    protected void onBindDialogView(View view) {
        super.onBindDialogView(view);

        String subtitle = dialogPreference.getDialogSubtitle();
        TextView subTitle = view.findViewById(R.id.summary);
        subTitle.setText(subtitle);
        subTitle.refreshDrawableState();
        mRadioGroup = view.findViewById(R.id.options);
        mDialogEntries = dialogPreference.getDialogEntries();

        int index = 0;

        for (CharSequence entry : mDialogEntries) {
            RadioButton radioButton = new RadioButton(getContext());
            radioButton.setLayoutParams(setParams());
            radioButton.setTextSize(15);
            radioButton.setText(entry);
            radioButton.setId(index);
            if (mRadioGroup != null) {
                mRadioGroup.addView(radioButton);
            }
            if (index == dialogPreference.getCheckedIndex()) {
                radioButton.setChecked(true);
            }
            index++;
        }
    }

    private RadioGroup.LayoutParams setParams() {
        RadioGroup.LayoutParams params = new RadioGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        float density = getResources().getDisplayMetrics().density;
        params.setMargins((int) (20 * density), (int) (10 * density), (int) (20 * density), 0);
        return params;
    }
}
