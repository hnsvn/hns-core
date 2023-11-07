/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.site_settings;

import android.os.Bundle;

import androidx.preference.Preference;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.settings.HnsPreferenceFragment;
import org.chromium.components.browser_ui.settings.SettingsUtils;

public class HnsWalletEthereumConnectedSites
        extends HnsPreferenceFragment implements Preference.OnPreferenceChangeListener {
    private static final String PREF_HNS_WALLET_ETHEREUM_CONNECTED_SITES =
            "pref_hns_wallet_ethereum_connected_sites";

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        getActivity().setTitle(R.string.settings_ethereum_title);
        SettingsUtils.addPreferencesFromResource(this, R.xml.hns_ethereum_preferences);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        return true;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        HnsWalletEthereumConnectedSitesPreference pref =
                (HnsWalletEthereumConnectedSitesPreference) findPreference(
                        PREF_HNS_WALLET_ETHEREUM_CONNECTED_SITES);
        pref.destroy();
    }
}
