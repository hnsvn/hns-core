/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.app.domain;

import android.content.Context;

import androidx.annotation.StringDef;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import org.chromium.hns_wallet.mojom.AccountInfo;
import org.chromium.hns_wallet.mojom.AllAccountsInfo;
import org.chromium.hns_wallet.mojom.HnsWalletConstants;
import org.chromium.hns_wallet.mojom.HnsWalletService;
import org.chromium.hns_wallet.mojom.CoinType;
import org.chromium.hns_wallet.mojom.KeyringId;
import org.chromium.hns_wallet.mojom.KeyringService;
import org.chromium.hns_wallet.mojom.KeyringServiceObserver;
import org.chromium.chrome.browser.crypto_wallet.observers.KeyringServiceObserverImpl;
import org.chromium.chrome.browser.crypto_wallet.util.AssetUtils;
import org.chromium.chrome.browser.crypto_wallet.util.WalletUtils;
import org.chromium.chrome.browser.util.LiveDataUtil;
import org.chromium.mojo.bindings.Callbacks;
import org.chromium.mojo.system.MojoException;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class KeyringModel implements KeyringServiceObserver {
    private final Object mLock = new Object();

    private Context mContext;
    private KeyringService mKeyringService;
    private HnsWalletService mHnsWalletService;
    private final MutableLiveData<AccountInfo> _mSelectedAccount;
    private final MutableLiveData<AllAccountsInfo> _mAllAccountsInfo;
    private final MutableLiveData<List<AccountInfo>> _mAccountInfos;
    private CryptoSharedActions mCryptoSharedActions;
    public LiveData<List<AccountInfo>> mAccountInfos;
    public LiveData<AccountInfo> mSelectedAccount;
    public LiveData<AllAccountsInfo> mAllAccountsInfo;

    public KeyringModel(Context context, KeyringService keyringService,
            HnsWalletService hnsWalletService, CryptoSharedActions cryptoSharedActions) {
        mContext = context;
        mKeyringService = keyringService;
        mHnsWalletService = hnsWalletService;
        mCryptoSharedActions = cryptoSharedActions;

        _mSelectedAccount = new MutableLiveData<>();
        mSelectedAccount = _mSelectedAccount;
        _mAllAccountsInfo = new MutableLiveData<>();
        mAllAccountsInfo = _mAllAccountsInfo;
        _mAccountInfos = new MutableLiveData<>(Collections.emptyList());
        mAccountInfos = _mAccountInfos;
    }

    public void init() {
        synchronized (mLock) {
            if (mKeyringService == null) {
                return;
            }
            mKeyringService.addObserver(this);
        }
    }

    public void update() {
        synchronized (mLock) {
            if (mKeyringService == null) {
                return;
            }
            mKeyringService.getAllAccounts(allAccounts -> {
                _mAllAccountsInfo.postValue(allAccounts);
                _mAccountInfos.postValue(Arrays.asList(allAccounts.accounts));
                _mSelectedAccount.postValue(allAccounts.selectedAccount);
            });
        }
    }

    public void setSelectedAccount(AccountInfo accountInfoToSelect) {
        synchronized (mLock) {
            if (mKeyringService == null || accountInfoToSelect == null) {
                return;
            }
            AccountInfo selectedAccount = _mSelectedAccount.getValue();
            if (selectedAccount != null
                    && WalletUtils.accountIdsEqual(selectedAccount, accountInfoToSelect)) {
                return;
            }

            mKeyringService.setSelectedAccount(accountInfoToSelect.accountId,
                    isAccountSelected -> { mCryptoSharedActions.updateCoinType(); });
        }
    }

    public void resetService(
            Context context, KeyringService keyringService, HnsWalletService hnsWalletService) {
        synchronized (mLock) {
            mContext = context;
            mKeyringService = keyringService;
            mHnsWalletService = hnsWalletService;
        }
        if (mKeyringService != null && mHnsWalletService != null) {
            init();
        }
    }

    public void getAccounts(Callbacks.Callback1<AccountInfo[]> callback1) {
        mKeyringService.getAllAccounts(allAccounts -> { callback1.call(allAccounts.accounts); });
    }

    private void addAccountInternal(@CoinType.EnumType int coinType,
            @KeyringId.EnumType int keyringId, String accountName,
            Callbacks.Callback1<Boolean> callback) {
        mKeyringService.addAccount(coinType, keyringId, accountName,
                result -> { handleAddAccountResult(result, callback); });
    }

    public void addAccount(@CoinType.EnumType int coinType, String chainId, String accountName,
            Callbacks.Callback1<Boolean> callback) {
        @KeyringId.EnumType
        int keyringId = AssetUtils.getKeyring(coinType, chainId);
        if (accountName == null) {
            LiveDataUtil.observeOnce(mAccountInfos, accounts -> {
                addAccountInternal(coinType, keyringId,
                        WalletUtils.generateUniqueAccountName(
                                mContext, coinType, accounts.toArray(new AccountInfo[0])),
                        callback);
            });
        } else {
            addAccountInternal(coinType, keyringId, accountName, callback);
        }
    }

    public void isWalletLocked(Callbacks.Callback1<Boolean> callback) {
        mKeyringService.isLocked(isWalletLocked -> callback.call(isWalletLocked));
    }

    public void registerKeyringObserver(KeyringServiceObserverImpl observer) {
        mKeyringService.addObserver(observer);
    }

    private void handleAddAccountResult(AccountInfo result, Callbacks.Callback1<Boolean> callback) {
        mCryptoSharedActions.updateCoinType();
        mCryptoSharedActions.onNewAccountAdded();
        callback.call(result != null);
    }

    @Override
    public void keyringCreated(@KeyringId.EnumType int keyringId) {
        update();
    }

    @Override
    public void keyringRestored(@KeyringId.EnumType int keyringId) {
        update();
    }

    @Override
    public void keyringReset() {
        update();
    }

    @Override
    public void locked() {
        update();
    }

    @Override
    public void unlocked() {
        update();
    }

    @Override
    public void backedUp() {
        update();
    }

    @Override
    public void accountsChanged() {
        update();
    }

    @Override
    public void accountsAdded(AccountInfo[] addedAccounts) {}

    @Override
    public void autoLockMinutesChanged() {}

    @Override
    public void selectedWalletAccountChanged(AccountInfo accountInfo) {
        update();
    }

    @Override
    public void selectedDappAccountChanged(
            @CoinType.EnumType int coinType, AccountInfo accountInfo) {
        update();
    }

    @Override
    public void onConnectionError(MojoException e) {}

    @Override
    public void close() {}

    @StringDef({HnsWalletConstants.FILECOIN_MAINNET, HnsWalletConstants.FILECOIN_TESTNET})
    public @interface FilecoinNetworkType {}
}
