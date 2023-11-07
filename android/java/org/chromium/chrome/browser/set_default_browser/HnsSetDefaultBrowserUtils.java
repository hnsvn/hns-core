/**
 * Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.set_default_browser;

import android.app.Activity;
import android.app.role.RoleManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.appcompat.app.AppCompatActivity;

import org.chromium.base.HnsPreferenceKeys;
import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.preferences.SharedPreferencesManager;
import org.chromium.chrome.browser.util.HnsConstants;
import org.chromium.components.embedder_support.util.UrlConstants;
import org.chromium.ui.widget.Toast;

public class HnsSetDefaultBrowserUtils {
    private static final String TAG = "BSDBrowserUtils";
    public static final String ANDROID_SETUPWIZARD_PACKAGE_NAME = "com.google.android.setupwizard";
    public static final String ANDROID_PACKAGE_NAME = "android";
    public static final String HNS_BLOG_URL = "https://hns.com/privacy-features/";

    public static boolean isBottomSheetVisible;

    public static boolean isHnsSetAsDefaultBrowser(Context context) {
        Intent browserIntent =
                new Intent(Intent.ACTION_VIEW, Uri.parse(UrlConstants.HTTP_URL_PREFIX));
        ResolveInfo resolveInfo = context.getPackageManager().resolveActivity(
                browserIntent, supportsDefault() ? PackageManager.MATCH_DEFAULT_ONLY : 0);
        if (resolveInfo == null || resolveInfo.activityInfo == null
                || resolveInfo.activityInfo.packageName == null
                || ContextUtils.getApplicationContext() == null) {
            return false;
        }

        if (context.getPackageName().equals(HnsConstants.HNS_PRODUCTION_PACKAGE_NAME)) {
            return resolveInfo.activityInfo.packageName.equals(
                    HnsConstants.HNS_PRODUCTION_PACKAGE_NAME);
        } else {
            return resolveInfo.activityInfo.packageName.equals(
                           HnsConstants.HNS_PRODUCTION_PACKAGE_NAME)
                    || resolveInfo.activityInfo.packageName.equals(
                            HnsConstants.HNS_BETA_PACKAGE_NAME)
                    || resolveInfo.activityInfo.packageName.equals(
                            HnsConstants.HNS_NIGHTLY_PACKAGE_NAME);
        }
    }

    public static void checkSetDefaultBrowserModal(AppCompatActivity activity) {
        if (!isHnsSetAsDefaultBrowser(activity) && !isHnsDefaultDontAsk()) {
            if (SharedPreferencesManager.getInstance().readInt(
                        HnsPreferenceKeys.HNS_APP_OPEN_COUNT)
                    == 5) {
                showHnsSetDefaultBrowserDialog(activity, false);

            } else if (shouldShowHnsWasDefaultDialog()) {
                int hnsWasDefaultCount = SharedPreferencesManager.getInstance().readInt(
                        HnsPreferenceKeys.HNS_WAS_DEFAULT_ASK_COUNT);
                SharedPreferencesManager.getInstance().writeInt(
                        HnsPreferenceKeys.HNS_WAS_DEFAULT_ASK_COUNT, hnsWasDefaultCount + 1);
                showHnsSetDefaultBrowserDialog(activity, false);
            }

        } else if (isHnsSetAsDefaultBrowser(activity) && !wasHnsDefaultBefore()) {
            setHnsDefaultSuccess();
        }
    }

    public static void showHnsSetDefaultBrowserDialog(
            AppCompatActivity activity, boolean isFromMenu) {
        /* (Albert Wang): Default app settings didn't get added until API 24
         * https://developer.android.com/reference/android/provider/Settings#ACTION_MANAGE_DEFAULT_APPS_SETTINGS
         */
        if (isHnsSetAsDefaultBrowser(activity)) {
            Toast toast = Toast.makeText(
                    activity, R.string.hns_already_set_as_default_browser, Toast.LENGTH_LONG);
            toast.show();
            return;
        }

        if (!isBottomSheetVisible) {
            isBottomSheetVisible = true;

            try {
                SetDefaultBrowserBottomSheetFragment bottomSheetDialog =
                        SetDefaultBrowserBottomSheetFragment.newInstance(isFromMenu);

                bottomSheetDialog.show(activity.getSupportFragmentManager(),
                        "SetDefaultBrowserBottomSheetFragment");
            } catch (IllegalStateException e) {
                // That exception could be thrown when Activity is not in the foreground.
                Log.e(TAG, "showHnsSetDefaultBrowserDialog error: " + e.getMessage());
                return;
            }

            if (!isFromMenu) {
                int hnsDefaultModalCount = SharedPreferencesManager.getInstance().readInt(
                        HnsPreferenceKeys.HNS_SET_DEFAULT_BOTTOM_SHEET_COUNT);
                SharedPreferencesManager.getInstance().writeInt(
                        HnsPreferenceKeys.HNS_SET_DEFAULT_BOTTOM_SHEET_COUNT,
                        hnsDefaultModalCount + 1);
            }
        }
    }

    public static void setDefaultBrowser(Activity activity) {
        int roleManagerOpenCount = SharedPreferencesManager.getInstance().readInt(
                HnsPreferenceKeys.HNS_ROLE_MANAGER_DIALOG_COUNT);

        if (supportsDefaultRoleManager() && roleManagerOpenCount < 2) {
            RoleManager roleManager = activity.getSystemService(RoleManager.class);

            if (roleManager.isRoleAvailable(RoleManager.ROLE_BROWSER)) {
                if (!roleManager.isRoleHeld(RoleManager.ROLE_BROWSER)) {
                    // save role manager open count as the second times onwards the dialog is shown,
                    // the system allows the user to click on "don't show again".
                    SharedPreferencesManager.getInstance().writeInt(
                            HnsPreferenceKeys.HNS_ROLE_MANAGER_DIALOG_COUNT,
                            roleManagerOpenCount + 1);

                    activity.startActivityForResult(
                            roleManager.createRequestRoleIntent(RoleManager.ROLE_BROWSER),
                            HnsConstants.DEFAULT_BROWSER_ROLE_REQUEST_CODE);
                }
            } else {
                openDefaultAppsSettings(activity);
            }

        } else if (supportsDefault()) {
            openDefaultAppsSettings(activity);

        } else {
            ResolveInfo resolveInfo = getResolveInfo(activity);
            if (resolveInfo.activityInfo.packageName.equals(ANDROID_SETUPWIZARD_PACKAGE_NAME)
                    || resolveInfo.activityInfo.packageName.equals(ANDROID_PACKAGE_NAME)) {
                openHnsBlog(activity);
            } else {
                Toast toast = Toast.makeText(
                        activity, R.string.hns_default_browser_go_to_settings, Toast.LENGTH_LONG);
                toast.show();
            }
        }
    }

    public static boolean supportsDefaultRoleManager() {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q;
    }

    private static boolean supportsDefault() {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.N;
    }

    private static ResolveInfo getResolveInfo(Activity activity) {
        Intent browserIntent =
                new Intent(Intent.ACTION_VIEW, Uri.parse(UrlConstants.HTTP_URL_PREFIX));

        return activity.getPackageManager().resolveActivity(
                browserIntent, supportsDefault() ? PackageManager.MATCH_DEFAULT_ONLY : 0);
    }

    private static void openDefaultAppsSettings(Activity activity) {
        if (activity instanceof OnHnsSetDefaultBrowserListener) {
            ((OnHnsSetDefaultBrowserListener) activity).OnCheckDefaultResume();
        }

        Intent intent = new Intent(Settings.ACTION_MANAGE_DEFAULT_APPS_SETTINGS);
        activity.startActivity(intent);
    }

    private static void openHnsBlog(Activity activity) {
        if (activity instanceof OnHnsSetDefaultBrowserListener) {
            ((OnHnsSetDefaultBrowserListener) activity).OnCheckDefaultResume();
        }

        LayoutInflater inflater = activity.getLayoutInflater();
        View layout = inflater.inflate(R.layout.hns_set_default_browser_dialog,
                (ViewGroup) activity.findViewById(R.id.hns_set_default_browser_toast_container));

        Toast toast = new Toast(activity, layout);
        toast.setDuration(Toast.LENGTH_LONG);
        toast.setGravity(Gravity.TOP, 0, 40);
        toast.show();

        Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(HNS_BLOG_URL));
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        activity.startActivity(intent);
    }

    private static boolean wasHnsDefaultBefore() {
        return SharedPreferencesManager.getInstance().readBoolean(
                HnsPreferenceKeys.HNS_IS_DEFAULT, false);
    }

    private static boolean shouldShowHnsWasDefaultDialog() {
        int hnsWasDefaultCount = SharedPreferencesManager.getInstance().readInt(
                HnsPreferenceKeys.HNS_WAS_DEFAULT_ASK_COUNT);
        return hnsWasDefaultCount < 2 && wasHnsDefaultBefore();
    }

    public static void setHnsDefaultSuccess() {
        SharedPreferencesManager.getInstance().writeBoolean(
                HnsPreferenceKeys.HNS_IS_DEFAULT, true);
    }

    public static void setHnsDefaultDontAsk() {
        SharedPreferencesManager.getInstance().writeBoolean(
                HnsPreferenceKeys.HNS_DEFAULT_DONT_ASK, true);
    }

    public static boolean isHnsDefaultDontAsk() {
        return SharedPreferencesManager.getInstance().readBoolean(
                HnsPreferenceKeys.HNS_DEFAULT_DONT_ASK, false);
    }
}
