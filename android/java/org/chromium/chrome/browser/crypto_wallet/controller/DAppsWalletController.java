/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet.controller;

import android.content.Context;
import android.content.DialogInterface;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.DefaultLifecycleObserver;
import androidx.lifecycle.LifecycleOwner;

import org.chromium.base.Log;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.hns_wallet.mojom.AssetRatioService;
import org.chromium.hns_wallet.mojom.HnsWalletService;
import org.chromium.hns_wallet.mojom.JsonRpcService;
import org.chromium.hns_wallet.mojom.KeyringService;
import org.chromium.chrome.browser.app.HnsActivity;
import org.chromium.chrome.browser.crypto_wallet.AssetRatioServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.HnsWalletServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.JsonRpcServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.KeyringServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.modal.HnsWalletPanel;
import org.chromium.chrome.browser.crypto_wallet.modal.DAppsDialog;
import org.chromium.chrome.browser.crypto_wallet.util.Utils;
import org.chromium.chrome.browser.fullscreen.BrowserControlsManager;
import org.chromium.chrome.browser.fullscreen.FullscreenManager;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarConfiguration;
import org.chromium.chrome.browser.util.ConfigurationUtils;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;

public class DAppsWalletController
        implements ConnectionErrorHandler, HnsWalletPanel.HnsWalletPanelServices {
    private static final String TAG = DAppsWalletController.class.getSimpleName();
    private FullscreenManager mFullscreenManager;
    private Context mContext;
    private View mAnchorViewHost;
    private AssetRatioService mAssetRatioService;
    private KeyringService mKeyringService;
    private HnsWalletService mHnsWalletService;
    protected JsonRpcService mJsonRpcService;
    private boolean mHasStateInitialise;
    private DAppsDialog mDAppsDialog;
    private HnsWalletPanel mHnsWalletPanel;
    private DialogInterface.OnDismissListener mOnDismissListener;
    private final AppCompatActivity mActivity;

    private DialogInterface.OnDismissListener mDialogOrPanelDismissListener = dialog -> {
        if (mOnDismissListener != null) {
            mOnDismissListener.onDismiss(dialog);
        }
        cleanUp();
    };

    public DAppsWalletController(Context mContext, View mAnchorViewHost) {
        this.mContext = mContext;
        this.mAnchorViewHost = mAnchorViewHost;
        this.mActivity = HnsActivity.getChromeTabbedActivity();
        try {
            HnsActivity activity = HnsActivity.getHnsActivity();
            ObservableSupplier<BrowserControlsManager> managerSupplier =
                    activity.getBrowserControlsManagerSupplier();
            mFullscreenManager = managerSupplier.get().getFullscreenManager();
        } catch (HnsActivity.HnsActivityNotFoundException | NullPointerException e) {
            Log.e(TAG, "Constructor", e);
        }
    }

    public DAppsWalletController(Context mContext, View mAnchorViewHost,
            DialogInterface.OnDismissListener onDismissListener) {
        this(mContext, mAnchorViewHost);
        this.mOnDismissListener = onDismissListener;
    }

    public void showWalletPanel() {
        InitAssetRatioService();
        InitKeyringService();
        InitJsonRpcService();
        InitHnsWalletService();
        if (Utils.shouldShowCryptoOnboarding()) {
            showOnBoardingOrUnlock();
        } else {
            mKeyringService.isLocked(isLocked -> {
                if (isLocked) {
                    showOnBoardingOrUnlock();
                } else {
                    boolean isFoundPendingDAppsTx = false;
                    // TODO: check if pending dapps transaction are available and implement an
                    // action accrodingly
                    if (!isFoundPendingDAppsTx) {
                        createAndShowWalletPanel();
                    }
                }
            });
        }
    }

    private void createAndShowWalletPanel() {
        mHnsWalletPanel =
                new HnsWalletPanel(mAnchorViewHost, mDialogOrPanelDismissListener, this);
        mHnsWalletPanel.showLikePopDownMenu();
        setupLifeCycleUpdater();
    }

    private void setupLifeCycleUpdater() {
        mActivity.getLifecycle().addObserver(defaultLifecycleObserver);
    }

    private void showOnBoardingOrUnlock() {
        int dialogStyle = DAppsDialog.DAppsDialogStyle.BOTTOM;
        if (mFullscreenManager != null && mFullscreenManager.getPersistentFullscreenMode()
                || shouldShowNotificationAtTop(mActivity)) {
            dialogStyle = DAppsDialog.DAppsDialogStyle.TOP;
        }
        mDAppsDialog =
                DAppsDialog.newInstance(mContext, mDialogOrPanelDismissListener, dialogStyle);
        mDAppsDialog.showOnboarding(Utils.shouldShowCryptoOnboarding());
    }

    @Override
    public AssetRatioService getAssetRatioService() {
        assert mAssetRatioService != null;
        return mAssetRatioService;
    }

    @Override
    public HnsWalletService getHnsWalletService() {
        assert mHnsWalletService != null;
        return mHnsWalletService;
    }

    @Override
    public KeyringService getKeyringService() {
        assert mKeyringService != null;
        return mKeyringService;
    }

    @Override
    public JsonRpcService getJsonRpcService() {
        assert mJsonRpcService != null;
        return mJsonRpcService;
    }

    @Override
    public void onConnectionError(MojoException e) {
        if (mKeyringService != null) {
            mKeyringService.close();
            mKeyringService = null;
        }
        if (mJsonRpcService != null) {
            mJsonRpcService.close();
            mJsonRpcService = null;
        }
        if (mHnsWalletService != null) {
            mHnsWalletService.close();
            mHnsWalletService = null;
        }
        if (mAssetRatioService != null) {
            mAssetRatioService.close();
            mAssetRatioService = null;
        }
        InitAssetRatioService();
        InitKeyringService();
        InitJsonRpcService();
        InitHnsWalletService();
        updateState();
    }

    public void dismiss() {
        if (isShowingPanel()) {
            mHnsWalletPanel.dismiss();
        }
        if (isShowingDialog()) {
            mDAppsDialog.dismiss();
        }
        mHnsWalletPanel = null;
        mDAppsDialog = null;
        cleanUp();
    }

    public boolean isShowingPanel() {
        return mHnsWalletPanel != null && mHnsWalletPanel.isShowing();
    }

    public boolean isShowingDialog() {
        return mDAppsDialog != null && mDAppsDialog.isShowing();
    }

    private void updateState() {
        if (mHasStateInitialise) return;
        // remember the dialog has been shown
        mHasStateInitialise = true;
    }

    private void InitKeyringService() {
        if (mKeyringService != null) {
            return;
        }
        mKeyringService = KeyringServiceFactory.getInstance().getKeyringService(this);
    }

    private void InitJsonRpcService() {
        if (mJsonRpcService != null) {
            return;
        }
        mJsonRpcService = JsonRpcServiceFactory.getInstance().getJsonRpcService(this);
    }

    private void InitHnsWalletService() {
        if (mHnsWalletService != null) {
            return;
        }
        mHnsWalletService = HnsWalletServiceFactory.getInstance().getHnsWalletService(this);
    }

    private void InitAssetRatioService() {
        if (mAssetRatioService != null) {
            return;
        }
        mAssetRatioService = AssetRatioServiceFactory.getInstance().getAssetRatioService(this);
    }

    private void cleanUp() {
        if (mKeyringService != null) {
            mKeyringService.close();
            mKeyringService = null;
        }
        if (mJsonRpcService != null) {
            mJsonRpcService.close();
            mJsonRpcService = null;
        }
        if (mHnsWalletService != null) {
            mHnsWalletService.close();
            mHnsWalletService = null;
        }
        if (mAssetRatioService != null) {
            mAssetRatioService.close();
            mAssetRatioService = null;
        }
        if (mActivity != null) {
            mActivity.getLifecycle().removeObserver(defaultLifecycleObserver);
        }
    }

    private final DefaultLifecycleObserver defaultLifecycleObserver =
            new DefaultLifecycleObserver() {
                @Override
                public void onResume(@NonNull LifecycleOwner owner) {
                    if (mHnsWalletPanel != null) {
                        mHnsWalletPanel.resume();
                    }
                }

                @Override
                public void onPause(@NonNull LifecycleOwner owner) {
                    if (mHnsWalletPanel != null) {
                        mHnsWalletPanel.pause();
                    }
                }
            };

    private boolean shouldShowNotificationAtTop(Context context) {
        return ConfigurationUtils.isTablet(context)
                || !BottomToolbarConfiguration.isBottomToolbarEnabled();
    }
}
