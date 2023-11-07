/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings.themes;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.RadioGroup;

import androidx.preference.PreferenceViewHolder;

import org.chromium.chrome.browser.HnsRelaunchUtils;
import org.chromium.chrome.browser.night_mode.R;
import org.chromium.chrome.browser.night_mode.settings.RadioButtonGroupThemePreference;
import org.chromium.chrome.browser.ntp_background_images.NTPBackgroundImagesBridge;
import org.chromium.chrome.browser.preferences.HnsPref;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.components.browser_ui.widget.RadioButtonWithDescription;
import org.chromium.components.user_prefs.UserPrefs;

public class HnsRadioButtonGroupCustomHomepageThemePreference extends RadioButtonGroupThemePreference {

    private RadioButtonWithDescription hnsDefaultView;
    private RadioButtonWithDescription refView;
    private NTPBackgroundImagesBridge mNTPBackgroundImagesBridge;

    public HnsRadioButtonGroupCustomHomepageThemePreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        Profile mProfile = Profile.getLastUsedRegularProfile();
        mNTPBackgroundImagesBridge = NTPBackgroundImagesBridge.getInstance(mProfile);
    }

    @Override
    public void onBindViewHolder(PreferenceViewHolder holder) {
        super.onBindViewHolder(holder);

        holder.findViewById(R.id.system_default).setVisibility(View.GONE);

        RadioButtonWithDescription hnsDefaultView = (RadioButtonWithDescription)holder.findViewById(R.id.light);
        RadioButtonWithDescription refView = (RadioButtonWithDescription)holder.findViewById(R.id.dark);
        if (mNTPBackgroundImagesBridge != null && mNTPBackgroundImagesBridge.isSuperReferral()) {
            refView.setPrimaryText(mNTPBackgroundImagesBridge.getSuperReferralThemeName());
            if(UserPrefs.get(Profile.getLastUsedRegularProfile()).getInteger(HnsPref.NEW_TAB_PAGE_SUPER_REFERRAL_THEMES_OPTION) == 1 ? true : false) {
                refView.setChecked(true);
                hnsDefaultView.setChecked(false);
            } else {
                refView.setChecked(false);
                hnsDefaultView.setChecked(true);
            }
        }
        hnsDefaultView.setPrimaryText(
                getContext().getResources().getString(org.chromium.chrome.R.string.hns_default));
    }

    @Override
    public void onCheckedChanged(RadioGroup group, int checkedId) {
        super.onCheckedChanged(group, checkedId);
        UserPrefs.get(Profile.getLastUsedRegularProfile()).setInteger(HnsPref.NEW_TAB_PAGE_SUPER_REFERRAL_THEMES_OPTION, checkedId == R.id.light ? 0 : 1 );
        HnsRelaunchUtils.askForRelaunch(getContext());
    }
}
