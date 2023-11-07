/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;

import androidx.preference.Preference;
import androidx.preference.PreferenceFragmentCompat;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.HnsRewardsNativeWorker;
import org.chromium.chrome.browser.ChromeTabbedActivity;
import org.chromium.chrome.browser.preferences.HnsPrefServiceBridge;
import org.chromium.chrome.browser.settings.developer.HnsRewardsDebugPreferences;

public class HnsPreferenceFragment extends PreferenceFragmentCompat {
    protected static final int STORAGE_PERMISSION_EXPORT_REQUEST_CODE = 8000;
    protected static final int STORAGE_PERMISSION_IMPORT_REQUEST_CODE = STORAGE_PERMISSION_EXPORT_REQUEST_CODE + 1;

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        menu.clear();
        inflater.inflate(R.menu.exit_settings_menu, menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == R.id.close_menu_id) {
            Intent intent = new Intent(getActivity(), ChromeTabbedActivity.class);
            intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
            intent.setAction(Intent.ACTION_VIEW);
            startActivity(intent);
        }
        return false;
    }

    @Override
    public void onCreatePreferences(Bundle bundle, String s) {
        setHasOptionsMenu(true);
    }

    @Override
    public void onResume() {
        super.onResume();
        HnsRewardsNativeWorker hnsRewardsNativeWorker = HnsRewardsNativeWorker.getInstance();
        if (hnsRewardsNativeWorker == null || !hnsRewardsNativeWorker.IsSupported()
                || HnsPrefServiceBridge.getInstance().getSafetynetCheckFailed()) {
            if (getPreferenceScreen() == null) return;
            Preference hnsRewardsDebugPreference =
                    getPreferenceScreen().findPreference(HnsRewardsDebugPreferences.KEY);
            if (hnsRewardsDebugPreference != null) {
                getPreferenceScreen().removePreference(hnsRewardsDebugPreference);
            }
        }
    }

    protected boolean isStoragePermissionGranted(boolean isExport) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            Context context = ContextUtils.getApplicationContext();
            if (context.checkSelfPermission(
                    android.Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED) {
                return true;
            } else {
                requestPermissions(
                        new String[] { android.Manifest.permission.WRITE_EXTERNAL_STORAGE },
                        isExport ? STORAGE_PERMISSION_EXPORT_REQUEST_CODE : STORAGE_PERMISSION_IMPORT_REQUEST_CODE);
                return false;
            }
        }
        return true;
    }
}
