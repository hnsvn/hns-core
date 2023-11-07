/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet;

import org.chromium.base.Callback;
import org.chromium.base.Log;
import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.hns_wallet.mojom.CoinType;
import org.chromium.chrome.browser.app.HnsActivity;
import org.chromium.chrome.browser.settings.HnsWalletPreferences;
import org.chromium.content_public.browser.WebContents;
import org.chromium.mojo.bindings.Callbacks;

@JNINamespace("hns_wallet")
public class HnsWalletProviderDelegateImplHelper {
    private static final String TAG = "HnsWalletProvider";

    @CalledByNative
    public static void showPanel() {
        try {
            HnsActivity activity = HnsActivity.getHnsActivity();
            activity.showWalletPanel(false);
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "showPanel " + e);
        }
    }

    @CalledByNative
    public static void showWalletOnboarding() {
        try {
            HnsActivity activity = HnsActivity.getHnsActivity();
            activity.showWalletOnboarding();
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "showWalletOnboarding " + e);
        }
    }

    @CalledByNative
    public static void walletInteractionDetected(WebContents webContents) {
        try {
            HnsActivity activity = HnsActivity.getHnsActivity();
            activity.walletInteractionDetected(webContents);
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "walletInteractionDetected " + e);
        }
    }

    @CalledByNative
    public static boolean isWeb3NotificationAllowed() {
        return HnsWalletPreferences.getPrefWeb3NotificationsEnabled();
    }

    @CalledByNative
    public static void ShowAccountCreation(@CoinType.EnumType int coinType) {
        try {
            HnsActivity activity = HnsActivity.getHnsActivity();
            activity.showAccountCreation(coinType);
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "ShowAccountCreation " + e);
        }
    }

    public static void IsSolanaConnected(
            WebContents webContents, String account, Callbacks.Callback1<Boolean> callback) {
        Callback<Boolean> callbackWrapper = result -> {
            callback.call(result);
        };
        HnsWalletProviderDelegateImplHelperJni.get().IsSolanaConnected(
                webContents, account, callbackWrapper);
    }

    @NativeMethods
    interface Natives {
        void IsSolanaConnected(WebContents webContents, String account, Callback<Boolean> callback);
    }
}
