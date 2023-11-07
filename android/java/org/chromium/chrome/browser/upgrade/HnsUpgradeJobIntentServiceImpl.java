/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.upgrade;

import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import org.chromium.base.Callback;
import org.chromium.base.Log;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskTraits;
import org.chromium.chrome.browser.HnsRewardsNativeWorker;
import org.chromium.chrome.browser.preferences.HnsPrefServiceBridge;
import org.chromium.chrome.browser.util.SafetyNetCheck;
import org.chromium.chrome.browser.util.TabUtils;
import org.chromium.content_public.browser.BrowserStartupController;

public class HnsUpgradeJobIntentServiceImpl extends HnsUpgradeJobIntentService.Impl {
    private static final String TAG = "HnsUpgradeJobIntentService";

    private static final int JOB_ID = 1;

    public static void maybePerformUpgradeTasks(Context context) {
        HnsUpgradeJobIntentServiceImpl.enqueueWork(context, new Intent());
    }

    private static void enqueueWork(Context context, Intent work) {
        HnsUpgradeJobIntentService.Impl.enqueueWork(
                context, HnsUpgradeJobIntentService.class, JOB_ID, work);
    }

    @Override
    protected void onHandleWork(@NonNull Intent intent) {
        PostTask.runOrPostTask(TaskTraits.UI_DEFAULT, () -> {
            BrowserStartupController.getInstance().addStartupCompletedObserver(
                    new BrowserStartupController.StartupCallback() {
                        @Override
                        public void onSuccess() {
                            HnsRewardsNativeWorker hnsRewardsNativeWorker =
                                    HnsRewardsNativeWorker.getInstance();
                            if (hnsRewardsNativeWorker != null
                                    && hnsRewardsNativeWorker.IsSupported()
                                    && HnsPrefServiceBridge.getInstance()
                                               .getSafetynetCheckFailed()) {
                                Callback<Boolean> callback = value -> {
                                    if (value == null || !value.booleanValue()) {
                                        return;
                                    }
                                    // Reset flag and update UI
                                    HnsPrefServiceBridge.getInstance().setSafetynetCheckFailed(
                                            false);
                                    TabUtils.enableRewardsButton();
                                };
                                // Re-perform safetynet check
                                SafetyNetCheck.updateSafetynetStatus(callback);
                            }
                        }

                        @Override
                        public void onFailure() {
                            Log.e(TAG,
                                    "Failed to perform upgrade tasks: BrowserStartupController.StartupCallback failed");
                        }
                    });
        });
    }
}
