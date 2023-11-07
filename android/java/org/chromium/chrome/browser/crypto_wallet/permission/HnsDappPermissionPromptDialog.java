/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet.permission;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Rect;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import org.chromium.base.Log;
import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.hns_wallet.mojom.AccountInfo;
import org.chromium.hns_wallet.mojom.HnsWalletService;
import org.chromium.hns_wallet.mojom.CoinType;
import org.chromium.hns_wallet.mojom.KeyringService;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.app.HnsActivity;
import org.chromium.chrome.browser.app.domain.WalletModel;
import org.chromium.chrome.browser.crypto_wallet.HnsWalletServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.KeyringServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.util.Utils;
import org.chromium.chrome.browser.crypto_wallet.util.WalletConstants;
import org.chromium.components.browser_ui.modaldialog.ModalDialogView;
import org.chromium.content_public.browser.ImageDownloadCallback;
import org.chromium.content_public.browser.WebContents;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;
import org.chromium.ui.LayoutInflaterUtils;
import org.chromium.ui.base.WindowAndroid;
import org.chromium.ui.modaldialog.DialogDismissalCause;
import org.chromium.ui.modaldialog.ModalDialogManager;
import org.chromium.ui.modaldialog.ModalDialogManager.ModalDialogType;
import org.chromium.ui.modaldialog.ModalDialogProperties;
import org.chromium.ui.modaldialog.ModalDialogProperties.ButtonType;
import org.chromium.ui.modelutil.PropertyModel;
import org.chromium.url.GURL;

import java.util.Iterator;
import java.util.List;

public class HnsDappPermissionPromptDialog
        implements ModalDialogProperties.Controller, ImageDownloadCallback, ConnectionErrorHandler {
    private static final String TAG = "HnsDappPermission";

    private final ModalDialogManager mModalDialogManager;
    private int mCoinType;
    private final Context mContext;
    private long mNativeDialogController;
    private PropertyModel mPropertyModel;
    private WebContents mWebContents;
    private String mFavIconURL;
    private ImageView mFavIconImage;
    private RecyclerView mRecyclerView;
    private HnsPermissionAccountsListAdapter mAccountsListAdapter;
    private int mRequestId; // Used for favicon downloader
    private KeyringService mKeyringService;
    private boolean mMojoServicesClosed;
    private HnsWalletService mHnsWalletService;
    private View mPermissionDialogPositiveButton;
    private WalletModel mWalletModel;

    @CalledByNative
    private static HnsDappPermissionPromptDialog create(long nativeDialogController,
            @NonNull WindowAndroid windowAndroid, WebContents webContents, String favIconURL,
            @CoinType.EnumType int coinType) {
        return new HnsDappPermissionPromptDialog(
                nativeDialogController, windowAndroid, webContents, favIconURL, coinType);
    }

    public HnsDappPermissionPromptDialog(long nativeDialogController, WindowAndroid windowAndroid,
            WebContents webContents, String favIconURL, @CoinType.EnumType int coinType) {
        mNativeDialogController = nativeDialogController;
        mWebContents = webContents;
        mFavIconURL = favIconURL;
        mContext = windowAndroid.getActivity().get();

        mModalDialogManager = windowAndroid.getModalDialogManager();
        mCoinType = coinType;
        mMojoServicesClosed = false;
        try {
            HnsActivity activity = HnsActivity.getHnsActivity();
            mWalletModel = activity.getWalletModel();
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "HnsDappPermissionPromptDialog constructor " + e);
        }
    }

    // @SuppressLint("DiscouragedApi") is required to suppress "getIdentifier" usage
    // The "container" view group is part of a dialog created by PropertyModel.Builder chromium API,
    // to which we don't have any direct access so there is no direct way to access the positive
    // button (as per my R&D) of the dialog created by PropertyModel.Builder. "getIdentifier" is the
    // only option to get the exact positive button's identifier value to change its state.
    @SuppressLint("DiscouragedApi")
    @CalledByNative
    void show() {
        View customView = LayoutInflaterUtils.inflate(
                mContext, R.layout.hns_permission_prompt_dialog, null);

        mFavIconImage = customView.findViewById(R.id.favicon);
        setFavIcon();
        mRecyclerView = customView.findViewById(R.id.accounts_list);

        InitHnsWalletService();
        TextView domain = customView.findViewById(R.id.domain);
        mHnsWalletService.getActiveOrigin(
                originInfo -> { domain.setText(Utils.geteTldSpanned(originInfo)); });

        mPropertyModel =
                new PropertyModel.Builder(ModalDialogProperties.ALL_KEYS)
                        .with(ModalDialogProperties.CONTROLLER, this)
                        .with(ModalDialogProperties.CUSTOM_VIEW, customView)
                        .with(ModalDialogProperties.POSITIVE_BUTTON_TEXT,
                                mContext.getString(
                                        R.string.permissions_connect_hns_wallet_connect_button_text))
                        .with(ModalDialogProperties.NEGATIVE_BUTTON_TEXT,
                                mContext.getString(
                                        R.string.permissions_connect_hns_wallet_back_button_text))
                        .with(ModalDialogProperties.FILTER_TOUCH_FOR_SECURITY, true)
                        .build();
        mModalDialogManager.showDialog(mPropertyModel, ModalDialogType.APP);
        InitKeyringService();
        try {
            HnsActivity activity = HnsActivity.getHnsActivity();
            activity.dismissWalletPanelOrDialog();

            ViewGroup container = getPermissionModalViewContainer(customView);
            mPermissionDialogPositiveButton =
                    container.findViewById(activity.getResources().getIdentifier(
                            WalletConstants.PERMISSION_DIALOG_POSITIVE_BUTTON_ID,
                            WalletConstants.RESOURCE_ID, activity.getPackageName()));
            if (mPermissionDialogPositiveButton != null) {
                mPermissionDialogPositiveButton.setEnabled(false);
            }
        } catch (HnsActivity.HnsActivityNotFoundException e) {
            Log.e(TAG, "show " + e);
        }
        initAccounts();
    }

    @NonNull
    private ViewGroup getPermissionModalViewContainer(View customView) {
        ViewParent viewParent = customView.getParent();
        while (viewParent.getParent() != null) {
            viewParent = viewParent.getParent();
            if (viewParent instanceof ModalDialogView) {
                break;
            }
        }
        return (ViewGroup) viewParent;
    }

    private void InitHnsWalletService() {
        if (mHnsWalletService != null) {
            return;
        }
        mHnsWalletService = HnsWalletServiceFactory.getInstance().getHnsWalletService(this);
    }

    @SuppressLint("NotifyDataSetChanged")
    private void initAccounts() {
        assert mKeyringService != null;
        assert mWalletModel != null;
        mAccountsListAdapter = new HnsPermissionAccountsListAdapter(new AccountInfo[0], true,
                new HnsPermissionAccountsListAdapter.HnsPermissionDelegate() {
                    @Override
                    public void onAccountCheckChanged(AccountInfo account, boolean isChecked) {
                        if (mPermissionDialogPositiveButton != null) {
                            mPermissionDialogPositiveButton.setEnabled(
                                    getSelectedAccounts().length > 0);
                        }
                    }
                });
        mRecyclerView.setAdapter(mAccountsListAdapter);
        LinearLayoutManager layoutManager = new LinearLayoutManager(mContext);
        mRecyclerView.setLayoutManager(layoutManager);
        mWalletModel.getDappsModel().fetchAccountsForConnectionReq(
                mCoinType, selectedAccountAllAccounts -> {
                    AccountInfo selectedAccount = selectedAccountAllAccounts.first;
                    List<AccountInfo> accounts = selectedAccountAllAccounts.second;
                    mAccountsListAdapter.setAccounts(accounts.toArray(new AccountInfo[0]));
                    if (accounts.size() > 0) {
                        mAccountsListAdapter.setSelectedAccount(selectedAccount);
                        if (mPermissionDialogPositiveButton != null) {
                            mPermissionDialogPositiveButton.setEnabled(true);
                        }
                    }
                    mAccountsListAdapter.notifyDataSetChanged();
                });
    }

    private void setFavIcon() {
        if (mFavIconURL.isEmpty()) {
            return;
        }
        mRequestId = mWebContents.downloadImage(new GURL(mFavIconURL), // url
                true, // isFavicon
                WalletConstants.MAX_BITMAP_SIZE_FOR_DOWNLOAD, // maxBitmapSize
                false, // bypassCache
                this); // callback
    }

    @Override
    public void onFinishDownloadImage(int id, int httpStatusCode, GURL imageUrl,
            List<Bitmap> bitmaps, List<Rect> originalImageSizes) {
        if (id != mRequestId) return;

        Iterator<Bitmap> iterBitmap = bitmaps.iterator();
        Iterator<Rect> iterSize = originalImageSizes.iterator();

        Bitmap bestBitmap = null;
        Rect bestSize = new Rect(0, 0, 0, 0);
        while (iterBitmap.hasNext() && iterSize.hasNext()) {
            Bitmap bitmap = iterBitmap.next();
            Rect size = iterSize.next();
            if (size.width() > bestSize.width() && size.height() > bestSize.height()) {
                bestBitmap = bitmap;
                bestSize = size;
            }
        }
        if (bestSize.width() == 0 || bestSize.height() == 0) {
            return;
        }

        mFavIconImage.setImageBitmap(bestBitmap);
        mFavIconImage.setVisibility(View.VISIBLE);
    }

    public String[] getSelectedAccounts() {
        assert mAccountsListAdapter != null;
        AccountInfo[] accountInfo = mAccountsListAdapter.getCheckedAccounts();
        String[] accounts = new String[accountInfo.length];
        for (int i = 0; i < accountInfo.length; i++) {
            accounts[i] = accountInfo[i].address;
        }

        return accounts;
    }

    @Override
    public void onClick(PropertyModel model, @ButtonType int buttonType) {
        if (buttonType == ButtonType.POSITIVE) {
            HnsDappPermissionPromptDialogJni.get().onPrimaryButtonClicked(
                    mNativeDialogController, getSelectedAccounts());
            mModalDialogManager.dismissDialog(
                    mPropertyModel, DialogDismissalCause.POSITIVE_BUTTON_CLICKED);
        } else if (buttonType == ButtonType.NEGATIVE) {
            HnsDappPermissionPromptDialogJni.get().onNegativeButtonClicked(
                    mNativeDialogController);
            mModalDialogManager.dismissDialog(
                    mPropertyModel, DialogDismissalCause.NEGATIVE_BUTTON_CLICKED);
        }
    }

    @Override
    public void onDismiss(PropertyModel model, int dismissalCause) {
        DisconnectMojoServices();
        HnsDappPermissionPromptDialogJni.get().onDialogDismissed(mNativeDialogController);
        mNativeDialogController = 0;
    }

    @CalledByNative
    private void dismissDialog() {
        mModalDialogManager.dismissDialog(mPropertyModel, DialogDismissalCause.DISMISSED_BY_NATIVE);
    }

    public void DisconnectMojoServices() {
        mMojoServicesClosed = true;
        if (mKeyringService != null) {
            mKeyringService.close();
            mKeyringService = null;
        }
        if (mHnsWalletService != null) {
            mHnsWalletService.close();
            mHnsWalletService = null;
        }
    }

    @Override
    public void onConnectionError(MojoException e) {
        if (mMojoServicesClosed || mKeyringService == null) {
            return;
        }
        mKeyringService.close();
        mKeyringService = null;
        InitKeyringService();
    }

    protected void InitKeyringService() {
        if (mKeyringService != null) {
            return;
        }

        mKeyringService = KeyringServiceFactory.getInstance().getKeyringService(this);
    }

    @NativeMethods
    interface Natives {
        void onPrimaryButtonClicked(
                long nativeHnsDappPermissionPromptDialogController, String[] accounts);
        void onNegativeButtonClicked(long nativeHnsDappPermissionPromptDialogController);
        void onDialogDismissed(long nativeHnsDappPermissionPromptDialogController);
    }
}
