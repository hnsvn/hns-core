/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.os.Bundle;

import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.HnsRewardsHelper;
import org.chromium.components.browser_ui.settings.SettingsUtils;

import java.io.IOException;
import java.io.InputStream;
import java.util.Scanner;

/**
 * Fragment to display Hns license information.
 */
public class HnsLicensePreferences extends HnsPreferenceFragment {
    private static final String TAG = "HnsLicense";

    private static final String PREF_HNS_LICENSE_TEXT = "hns_license_text";
    private static final String ASSET_HNS_LICENSE = "LICENSE.html";

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String s) {
        // These strings are not used in Hns, but we get them from automated string translation
        // process. These checks are just making sure that strings are still used in Chromium to
        // avoid lint issues.
        assert R.string.chrome_additional_terms_of_service_title > 0
            : "Something has changed in the upstream!";
        assert R.string.google_privacy_policy_url > 0 : "Something has changed in the upstream!";
        assert R.string.sync_reading_list > 0 : "Something has changed in the upstream!";

        SettingsUtils.addPreferencesFromResource(this, R.xml.hns_license_preferences);
        getActivity().setTitle(R.string.hns_license_text);
        HnsLicensePreference licenseText =
                (HnsLicensePreference) findPreference(PREF_HNS_LICENSE_TEXT);
        try {
            InputStream in = getActivity().getAssets().open(ASSET_HNS_LICENSE);
            Scanner scanner = new Scanner(in).useDelimiter("\\A");
            String summary = scanner.hasNext() ? scanner.next() : "";
            in.close();
            licenseText.setSummary(HnsRewardsHelper.spannedFromHtmlString(summary));
        } catch (IOException e) {
            Log.e(TAG, "Could not load license text: " + e);
        }
    }
}
