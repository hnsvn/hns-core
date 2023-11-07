/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.language.settings;

import android.os.Bundle;

import androidx.preference.PreferenceCategory;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.HnsRelaunchUtils;
import org.chromium.chrome.browser.preferences.HnsPref;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.user_prefs.UserPrefs;

public class HnsLanguageSettings extends LanguageSettings {
    static final String TRANSLATION_SETTINGS_SECTION = "translation_settings_section";
    static final String APP_LANGUAGE_SECTION = "app_language_section";

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        super.onCreatePreferences(savedInstanceState, rootKey);

        PreferenceCategory translateSwitch =
                (PreferenceCategory) findPreference(TRANSLATION_SETTINGS_SECTION);
        if (translateSwitch != null) {
            getPreferenceScreen().removePreference(translateSwitch);
        }

        PreferenceCategory appLanguageSection =
                (PreferenceCategory) findPreference(APP_LANGUAGE_SECTION);
        if (appLanguageSection != null) {
            boolean isHnsTranslateEnabled =
                    UserPrefs.get(Profile.getLastUsedRegularProfile())
                            .getBoolean(HnsPref.OFFER_TRANSLATE_ENABLED);
            ChromeSwitchPreference hnsTranslateFeaturePreference =
                    new ChromeSwitchPreference(getContext());
            hnsTranslateFeaturePreference.setTitle(
                    getResources().getString(R.string.use_hns_translate));
            hnsTranslateFeaturePreference.setChecked(isHnsTranslateEnabled);
            hnsTranslateFeaturePreference.setOnPreferenceChangeListener(
                    (preference, newValue) -> {
                        UserPrefs.get(Profile.getLastUsedRegularProfile())
                                .setBoolean(HnsPref.OFFER_TRANSLATE_ENABLED, (boolean) newValue);
                        if (getActivity() != null) {
                            HnsRelaunchUtils.askForRelaunch(getActivity());
                        }
                        return true;
                    });
            appLanguageSection.addPreference(hnsTranslateFeaturePreference);
        }
    }
}
