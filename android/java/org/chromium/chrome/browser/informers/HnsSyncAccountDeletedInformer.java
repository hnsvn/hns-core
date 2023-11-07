/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.informers;

import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.base.annotations.CalledByNative;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.HnsSyncWorker;
import org.chromium.chrome.browser.app.HnsActivity;
import org.chromium.chrome.browser.infobar.HnsInfoBarIdentifier;
import org.chromium.chrome.browser.settings.HnsSyncScreensPreference;
import org.chromium.chrome.browser.settings.SettingsLauncherImpl;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.ui.messages.infobar.HnsSimpleConfirmInfoBarBuilder;
import org.chromium.chrome.browser.ui.messages.infobar.SimpleConfirmInfoBarBuilder;
import org.chromium.components.browser_ui.settings.SettingsLauncher;

public class HnsSyncAccountDeletedInformer {
    private static final String TAG = "SyncAccountDeleted";

    @CalledByNative
    public static void show() {
        try {
            HnsActivity activity = HnsActivity.getHnsActivity();

            Tab tab = activity.getActivityTabProvider().get();
            if (tab == null) return;

            if (!HnsSyncWorker.get().isAccountDeletedNoticePending()) {
                return;
            }

            HnsSimpleConfirmInfoBarBuilder.createInfobarWithDrawable(tab.getWebContents(),
                    new SimpleConfirmInfoBarBuilder.Listener() {
                        @Override
                        public void onInfoBarDismissed() {
                            // Pressing cross
                            // In any way don't show the informer again
                            disableInformer();
                        }

                        @Override
                        public boolean onInfoBarButtonClicked(boolean isPrimary) {
                            assert isPrimary : "We don't have secondary button";
                            // Pressing `OK`
                            // Don't show the informer again
                            disableInformer();
                            return false;
                        }

                        @Override
                        public boolean onInfoBarLinkClicked() {
                            // Pressing link `re-create the account`
                            // Don't show the informer again
                            disableInformer();
                            SettingsLauncher settingsLauncher = new SettingsLauncherImpl();
                            settingsLauncher.launchSettingsActivity(
                                    ContextUtils.getApplicationContext(),
                                    HnsSyncScreensPreference.class);
                            return false;
                        }
                    },
                    HnsInfoBarIdentifier.HNS_SYNC_ACCOUNT_DELETED_INFOBAR, activity,
                    R.drawable.ic_warning_circle,
                    // See comment at |HnsSyncAccountDeletedInfoBarDelegate::GetMessageText|
                    // for the informer text and link test placeholder empty substitution
                    activity.getString(R.string.hns_sync_account_deleted_infobar_message, ""),
                    activity.getString(R.string.ok), "",
                    activity.getString(R.string.hns_sync_account_deleted_infobar_link_text, ""),
                    false);
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "show " + e);
        }
    }

    private static void disableInformer() {
        HnsSyncWorker.get().clearAccountDeletedNoticePending();
    }
}
