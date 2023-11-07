/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import org.chromium.chrome.browser.download.settings.HnsDownloadSettings;
import org.chromium.chrome.browser.download.settings.DownloadSettings;
import org.chromium.chrome.browser.safe_browsing.settings.HnsStandardProtectionSettingsFragment;
import org.chromium.chrome.browser.safe_browsing.settings.StandardProtectionSettingsFragment;

public class HnsSettingsLauncherImpl extends SettingsLauncherImpl {
    public HnsSettingsLauncherImpl() {
        super();
    }

    @Override
    public void launchSettingsActivity(Context context,
            @Nullable Class<? extends Fragment> fragment, @Nullable Bundle fragmentArgs) {
        if (fragment != null) {
            // Substitute with our version of class
            if (fragment.equals(StandardProtectionSettingsFragment.class)) {
                fragment = HnsStandardProtectionSettingsFragment.class;
            } else if (fragment.equals(DownloadSettings.class)) {
                fragment = HnsDownloadSettings.class;
            }
        }
        super.launchSettingsActivity(context, fragment, fragmentArgs);
    }

    @Override
    public Intent createSettingsActivityIntent(
            Context context, @Nullable String fragmentName, @Nullable Bundle fragmentArgs) {
        Intent intent = super.createSettingsActivityIntent(context, fragmentName, fragmentArgs);
        intent.setClass(context, HnsSettingsActivity.class);
        if (!(context instanceof Activity)) {
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        }
        if (fragmentName != null) {
            intent.putExtra(SettingsActivity.EXTRA_SHOW_FRAGMENT, fragmentName);
        }
        if (fragmentArgs != null) {
            intent.putExtra(SettingsActivity.EXTRA_SHOW_FRAGMENT_ARGUMENTS, fragmentArgs);
        }
        return intent;
    }
}
