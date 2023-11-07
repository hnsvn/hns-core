/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.upgrade;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import org.chromium.base.HnsPreferenceKeys;
import org.chromium.chrome.browser.notifications.retention.RetentionNotificationUtil;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.preferences.SharedPreferencesManager;
import org.chromium.chrome.browser.util.HnsConstants;

/**
 * Triggered when Hns's package is replaced (e.g. when it is
 * upgraded).
 *
 * See important lifecycle notes in PackageReplacedBroadcastReceiver.
 */
public final class HnsPackageReplacedBroadcastReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(final Context context, Intent intent) {
        if (!Intent.ACTION_MY_PACKAGE_REPLACED.equals(intent.getAction())) return;
        HnsUpgradeJobIntentServiceImpl.maybePerformUpgradeTasks(context);
        try {
            SharedPreferencesManager.getInstance().writeInt(HnsPreferenceKeys.HNS_APP_OPEN_COUNT, 0);
            // Set dormant users notifications
            if (OnboardingPrefManager.getInstance().isDormantUsersEngagementEnabled()
                    || context.getPackageName().equals(
                            HnsConstants.HNS_PRODUCTION_PACKAGE_NAME)) {
                OnboardingPrefManager.getInstance().setDormantUsersPrefs();
                if (!OnboardingPrefManager.getInstance().isDormantUsersNotificationsStarted()) {
                    RetentionNotificationUtil.scheduleDormantUsersNotifications(context);
                    OnboardingPrefManager.getInstance().setDormantUsersNotificationsStarted(true);
                }
            }
        } catch (Exception exc) {
            // Sometimes the pref is not registered yet in the app
        }
        // try {
        //     NotificationIntent.fireNotificationIfNecessary(context);
        // } catch (Exception exc) {
        //     // Just ignore if we could not send a notification
        // }
    }
}
