/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.app.HnsActivity;
import org.chromium.chrome.browser.infobar.HnsInfoBarIdentifier;
import org.chromium.chrome.browser.settings.HnsSyncScreensPreference;
import org.chromium.chrome.browser.settings.SettingsLauncherImpl;
import org.chromium.chrome.browser.sync.SyncServiceFactory;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.ui.messages.infobar.HnsSimpleConfirmInfoBarBuilder;
import org.chromium.chrome.browser.ui.messages.infobar.SimpleConfirmInfoBarBuilder;
import org.chromium.components.browser_ui.settings.SettingsLauncher;

public class HnsSyncInformers {
    private static final String TAG = "HnsSyncInformers";

    public static void show() {
        showSetupV2IfRequired();
    }

    private static void showSetupV2IfRequired() {
        HnsSyncWorker hnsSyncWorker = HnsSyncWorker.get();
        boolean wasV1User = hnsSyncWorker.getSyncV1WasEnabled();

        if (!wasV1User) {
            return;
        }

        boolean infobarDismissed = hnsSyncWorker.getSyncV2MigrateNoticeDismissed();
        if (infobarDismissed) {
            return;
        }

        boolean isV2User = SyncServiceFactory.get() != null
                && SyncServiceFactory.get().isInitialSyncFeatureSetupComplete();
        if (isV2User) {
            hnsSyncWorker.setSyncV2MigrateNoticeDismissed(true);
            return;
        }

        showSyncV2NeedsSetup();
    }

    public static void showSyncV2NeedsSetup() {
        HnsActivity activity = null;
        try {
            activity = HnsActivity.getHnsActivity();
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "showSyncV2NeedsSetup " + e);
            return;
        }

        Tab tab = activity.getActivityTabProvider().get();
        if (tab == null) return;

        HnsSimpleConfirmInfoBarBuilder.createInfobarWithDrawable(tab.getWebContents(),
                new SimpleConfirmInfoBarBuilder.Listener() {
                    @Override
                    public void onInfoBarDismissed() {
                    }

                    @Override
                    public boolean onInfoBarButtonClicked(boolean isPrimary) {
                        if (isPrimary) {
                            SettingsLauncher settingsLauncher = new SettingsLauncherImpl();
                            settingsLauncher.launchSettingsActivity(
                                ContextUtils.getApplicationContext(),
                                HnsSyncScreensPreference.class);
                        }
                        return false;
                    }

                    @Override
                    public boolean onInfoBarLinkClicked() {
                        return false;
                    }
                },
                HnsInfoBarIdentifier.SYNC_V2_MIGRATE_INFOBAR_DELEGATE, activity,
                R.drawable.sync_icon /* drawableId */,
                activity.getString(R.string.hns_sync_v2_migrate_infobar_message) /* message */,
                activity.getString(
                        R.string.hns_sync_v2_migrate_infobar_command) /* primaryText */,
                null /* secondaryText */, null /* linkText */, false /* autoExpire */);
        HnsSyncWorker.get().setSyncV2MigrateNoticeDismissed(true);
    }
}
