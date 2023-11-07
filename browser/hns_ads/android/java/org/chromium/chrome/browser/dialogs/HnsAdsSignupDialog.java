/**
 * Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.dialogs;

import android.app.AlarmManager;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager.NameNotFoundException;
import android.view.View;
import android.widget.ImageView;

import org.chromium.base.ContextUtils;
import org.chromium.base.IntentUtils;
import org.chromium.base.Log;
import org.chromium.base.annotations.CalledByNative;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.HnsRewardsNativeWorker;
import org.chromium.chrome.browser.app.HnsActivity;
import org.chromium.chrome.browser.notifications.HnsOnboardingNotification;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.settings.AppearancePreferences;
import org.chromium.chrome.browser.util.PackageUtils;

public class HnsAdsSignupDialog {
    private static final String TAG = "HnsAdsSignupDialog";

    private static String SHOULD_SHOW_ONBOARDING_DIALOG_VIEW_COUNTER = "should_show_onboarding_dialog_view_counter";
    private static String SHOULD_SHOW_ONBOARDING_DIALOG = "should_show_onboarding_dialog";

    private static final long TWENTY_FOUR_HOURS = 86_400_000;
    private static final long MOMENT_LATER = 2_500;

    public static boolean shouldShowNewUserDialog(Context context) {
        HnsRewardsNativeWorker hnsRewardsNativeWorker = HnsRewardsNativeWorker.getInstance();
        boolean shouldShow = shouldShowOnboardingDialog() && PackageUtils.isFirstInstall(context)
                && hasElapsed24Hours(context) && hnsRewardsNativeWorker != null
                && !hnsRewardsNativeWorker.isRewardsEnabled()
                && hnsRewardsNativeWorker.IsSupported();

        boolean shouldShowForViewCount = shouldShowForViewCount();
        if (shouldShow) updateViewCount();

        return shouldShow && shouldShowForViewCount;
    }

    public static boolean shouldShowNewUserDialogIfRewardsIsSwitchedOff(Context context) {
        HnsRewardsNativeWorker hnsRewardsNativeWorker = HnsRewardsNativeWorker.getInstance();
        boolean shouldShow = shouldShowOnboardingDialog() && !PackageUtils.isFirstInstall(context)
                && hnsRewardsNativeWorker != null && !hnsRewardsNativeWorker.isRewardsEnabled()
                && hnsRewardsNativeWorker.IsSupported();

        boolean shouldShowForViewCount = shouldShowForViewCount();
        if (shouldShow) updateViewCount();

        return shouldShow && shouldShowForViewCount;
    }

    public static boolean shouldShowExistingUserDialog(Context context) {
        HnsRewardsNativeWorker hnsRewardsNativeWorker = HnsRewardsNativeWorker.getInstance();
        boolean shouldShow = shouldShowOnboardingDialog() && !PackageUtils.isFirstInstall(context)
                && hnsRewardsNativeWorker != null && !hnsRewardsNativeWorker.isRewardsEnabled()
                && hnsRewardsNativeWorker.IsSupported();

        boolean shouldShowForViewCount = shouldShowForViewCount();
        if (shouldShow) updateViewCount();

        return shouldShow && shouldShowForViewCount;
    }

    @CalledByNative
    public static void enqueueOnboardingNotificationNative(boolean useCustomNotifications) {
        enqueueOnboardingNotification(ContextUtils.getApplicationContext(), useCustomNotifications);
    }

    @CalledByNative
    public static boolean showAdsInBackground() {
        return AppearancePreferences.getPrefAdsInBackgroundEnabled();
    }

    private static void enqueueOnboardingNotification(Context context, boolean useCustomNotification) {
        if(!OnboardingPrefManager.getInstance().isOnboardingNotificationShown()) {
            AlarmManager am = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
            Intent intent = new Intent(context, HnsOnboardingNotification.class);
            intent.putExtra(HnsOnboardingNotification.USE_CUSTOM_NOTIFICATION, useCustomNotification);
            am.set(AlarmManager.RTC_WAKEUP, System.currentTimeMillis() + MOMENT_LATER,
                    PendingIntent.getBroadcast(context, 0, intent,
                            PendingIntent.FLAG_UPDATE_CURRENT
                                    | IntentUtils.getPendingIntentMutabilityFlag(true)));
        }
    }

    public static void showNewUserDialog(Context context) {
        AlertDialog alertDialog = new AlertDialog.Builder(context, R.style.HnsDialogTheme)
        .setView(R.layout.hns_ads_new_user_dialog_layout)
        .setPositiveButton(R.string.hns_ads_offer_positive, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {

                OnboardingPrefManager.getInstance().setOnboardingNotificationShown(false);

                neverShowOnboardingDialogAgain();
                try {
                    // Enable ads
                    HnsActivity.getHnsActivity().openRewardsPanel();
                } catch (HnsActivity.HnsActivityNotFoundException e) {
                    Log.e(TAG, "showNewUserDialog " + e);
                }
            }
        }).create();
        alertDialog.show();

        ImageView closeButton = alertDialog.findViewById(R.id.close_button);
        closeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                alertDialog.cancel();
            }
        });
    }

    public static void showExistingUserDialog(Context context) {
        AlertDialog alertDialog = new AlertDialog.Builder(context, R.style.HnsDialogTheme)
        .setView(R.layout.hns_ads_existing_user_dialog_layout)
        .setPositiveButton(R.string.hns_ads_offer_positive, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                // Enable ads
                neverShowOnboardingDialogAgain();
                try {
                    HnsActivity hnsActivity = HnsActivity.getHnsActivity();
                    hnsActivity.openRewardsPanel();
                } catch (HnsActivity.HnsActivityNotFoundException e) {
                    Log.e(TAG, "showExistingUserDialog " + e);
                }
            }
        }).create();
        alertDialog.show();

        ImageView closeButton = alertDialog.findViewById(R.id.close_button);
        closeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                alertDialog.cancel();
            }
        });
    }

    private static boolean hasElapsed24Hours(Context context) {
        boolean result = false;
        try {
            result = System.currentTimeMillis() >= context.getPackageManager().getPackageInfo(context.getPackageName(), 0).firstInstallTime + TWENTY_FOUR_HOURS;
        } catch (NameNotFoundException e) {}
        return result;
    }

    private static boolean shouldShowForViewCount() {
        SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();

        int viewCount = sharedPref.getInt(SHOULD_SHOW_ONBOARDING_DIALOG_VIEW_COUNTER, 0);
        return 0 == viewCount || 20 == viewCount || 40 == viewCount;
    }

    private static void updateViewCount() {
        SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
        SharedPreferences.Editor editor = sharedPref.edit();

        editor.putInt(SHOULD_SHOW_ONBOARDING_DIALOG_VIEW_COUNTER, sharedPref.getInt(SHOULD_SHOW_ONBOARDING_DIALOG_VIEW_COUNTER, 0) + 1);
        editor.apply();
    }

    private static void neverShowOnboardingDialogAgain() {
        SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
        SharedPreferences.Editor editor = sharedPref.edit();

        editor.putBoolean(SHOULD_SHOW_ONBOARDING_DIALOG, false);
        editor.apply();
    }

    private static boolean shouldShowOnboardingDialog() {
        SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();

        return sharedPref.getBoolean(SHOULD_SHOW_ONBOARDING_DIALOG, true);
    }
}
