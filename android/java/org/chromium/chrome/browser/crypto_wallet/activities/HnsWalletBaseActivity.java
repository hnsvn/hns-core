/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet.activities;

import android.view.MenuItem;

import org.chromium.hns_wallet.mojom.AssetRatioService;
import org.chromium.hns_wallet.mojom.BlockchainRegistry;
import org.chromium.hns_wallet.mojom.HnsWalletP3a;
import org.chromium.hns_wallet.mojom.HnsWalletService;
import org.chromium.hns_wallet.mojom.EthTxManagerProxy;
import org.chromium.hns_wallet.mojom.JsonRpcService;
import org.chromium.hns_wallet.mojom.KeyringService;
import org.chromium.hns_wallet.mojom.SolanaTxManagerProxy;
import org.chromium.hns_wallet.mojom.TransactionInfo;
import org.chromium.hns_wallet.mojom.TxService;
import org.chromium.chrome.browser.crypto_wallet.AssetRatioServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.BlockchainRegistryFactory;
import org.chromium.chrome.browser.crypto_wallet.HnsWalletServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.JsonRpcServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.KeyringServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.TxServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.observers.KeyringServiceObserverImpl;
import org.chromium.chrome.browser.crypto_wallet.observers.KeyringServiceObserverImpl.KeyringServiceObserverImplDelegate;
import org.chromium.chrome.browser.crypto_wallet.observers.TxServiceObserverImpl;
import org.chromium.chrome.browser.crypto_wallet.observers.TxServiceObserverImpl.TxServiceObserverImplDelegate;
import org.chromium.chrome.browser.init.AsyncInitializationActivity;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;

public abstract class HnsWalletBaseActivity extends AsyncInitializationActivity
        implements ConnectionErrorHandler, KeyringServiceObserverImplDelegate,
                   TxServiceObserverImplDelegate {
    protected KeyringService mKeyringService;
    protected BlockchainRegistry mBlockchainRegistry;
    protected JsonRpcService mJsonRpcService;
    protected TxService mTxService;
    protected EthTxManagerProxy mEthTxManagerProxy;
    protected SolanaTxManagerProxy mSolanaTxManagerProxy;
    protected AssetRatioService mAssetRatioService;
    protected HnsWalletP3a mHnsWalletP3A;
    protected HnsWalletService mHnsWalletService;
    private KeyringServiceObserverImpl mKeyringServiceObserver;
    private TxServiceObserverImpl mTxServiceObserver;

    @Override
    public void onUserInteraction() {
        if (mKeyringService == null) {
            return;
        }
        mKeyringService.notifyUserInteraction();
    }

    @Override
    public void onConnectionError(MojoException e) {
        if (mKeyringServiceObserver != null) {
            mKeyringServiceObserver.close();
            mKeyringServiceObserver = null;
        }
        if (mTxServiceObserver != null) {
            mTxServiceObserver.close();
            mTxServiceObserver.destroy();
            mTxServiceObserver = null;
        }
        if (mKeyringService != null) mKeyringService.close();
        if (mAssetRatioService != null) mAssetRatioService.close();
        if (mBlockchainRegistry != null) mBlockchainRegistry.close();
        if (mJsonRpcService != null) mJsonRpcService.close();
        if (mTxService != null) mTxService.close();
        if (mEthTxManagerProxy != null) mEthTxManagerProxy.close();
        if (mSolanaTxManagerProxy != null) mSolanaTxManagerProxy.close();
        if (mHnsWalletP3A != null) mHnsWalletP3A.close();
        if (mHnsWalletService != null) mHnsWalletService.close();

        mKeyringService = null;
        mBlockchainRegistry = null;
        mJsonRpcService = null;
        mTxService = null;
        mEthTxManagerProxy = null;
        mSolanaTxManagerProxy = null;
        mAssetRatioService = null;
        mHnsWalletP3A = null;
        mHnsWalletService = null;
        InitKeyringService();
        InitBlockchainRegistry();
        InitJsonRpcService();
        InitTxService();
        InitEthTxManagerProxy();
        InitSolanaTxManagerProxy();
        InitAssetRatioService();
        InitHnsWalletP3A();
        InitHnsWalletService();
    }

    protected void InitTxService() {
        if (mTxService != null) {
            return;
        }

        mTxService = TxServiceFactory.getInstance().getTxService(this);
        mTxServiceObserver = new TxServiceObserverImpl(this);
        mTxService.addObserver(mTxServiceObserver);
    }

    protected void InitEthTxManagerProxy() {
        if (mEthTxManagerProxy != null) {
            return;
        }

        mEthTxManagerProxy = TxServiceFactory.getInstance().getEthTxManagerProxy(this);
    }

    protected void InitSolanaTxManagerProxy() {
        if (mSolanaTxManagerProxy != null) {
            return;
        }

        mSolanaTxManagerProxy = TxServiceFactory.getInstance().getSolanaTxManagerProxy(this);
    }

    protected void InitKeyringService() {
        if (mKeyringService != null) {
            return;
        }

        mKeyringService = KeyringServiceFactory.getInstance().getKeyringService(this);
        mKeyringServiceObserver = new KeyringServiceObserverImpl(this);
        mKeyringService.addObserver(mKeyringServiceObserver);
    }

    protected void InitBlockchainRegistry() {
        if (mBlockchainRegistry != null) {
            return;
        }

        mBlockchainRegistry = BlockchainRegistryFactory.getInstance().getBlockchainRegistry(this);
    }

    protected void InitJsonRpcService() {
        if (mJsonRpcService != null) {
            return;
        }

        mJsonRpcService = JsonRpcServiceFactory.getInstance().getJsonRpcService(this);
    }

    protected void InitAssetRatioService() {
        if (mAssetRatioService != null) {
            return;
        }

        mAssetRatioService = AssetRatioServiceFactory.getInstance().getAssetRatioService(this);
    }

    protected void InitHnsWalletP3A() {
        if (mHnsWalletP3A != null) {
            return;
        }

        mHnsWalletP3A = HnsWalletServiceFactory.getInstance().getHnsWalletP3A(this);
    }

    protected void InitHnsWalletService() {
        if (mHnsWalletService != null) {
            return;
        }

        mHnsWalletService = HnsWalletServiceFactory.getInstance().getHnsWalletService(this);
    }

    public KeyringService getKeyringService() {
        return mKeyringService;
    }

    public BlockchainRegistry getBlockchainRegistry() {
        return mBlockchainRegistry;
    }

    public JsonRpcService getJsonRpcService() {
        return mJsonRpcService;
    }

    public TxService getTxService() {
        return mTxService;
    }

    public EthTxManagerProxy getEthTxManagerProxy() {
        return mEthTxManagerProxy;
    }

    public SolanaTxManagerProxy getSolanaTxManagerProxy() {
        return mSolanaTxManagerProxy;
    }

    public AssetRatioService getAssetRatioService() {
        return mAssetRatioService;
    }

    public HnsWalletService getHnsWalletService() {
        return mHnsWalletService;
    }

    public HnsWalletP3a getHnsWalletP3A() {
        return mHnsWalletP3A;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                finish();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public void finishNativeInitialization() {
        super.finishNativeInitialization();
        InitKeyringService();
        InitBlockchainRegistry();
        InitJsonRpcService();
        InitTxService();
        InitEthTxManagerProxy();
        InitSolanaTxManagerProxy();
        InitAssetRatioService();
        InitHnsWalletP3A();
        InitHnsWalletService();
    }

    @Override
    public void onDestroy() {
        if (mKeyringServiceObserver != null) {
            mKeyringServiceObserver.close();
        }
        if (mTxServiceObserver != null) {
            mTxServiceObserver.close();
            mTxServiceObserver.destroy();
        }
        if (mKeyringService != null) mKeyringService.close();
        if (mAssetRatioService != null) mAssetRatioService.close();
        if (mBlockchainRegistry != null) mBlockchainRegistry.close();
        if (mJsonRpcService != null) mJsonRpcService.close();
        if (mTxService != null) mTxService.close();
        if (mEthTxManagerProxy != null) mEthTxManagerProxy.close();
        if (mSolanaTxManagerProxy != null) mSolanaTxManagerProxy.close();
        if (mHnsWalletP3A != null) mHnsWalletP3A.close();
        if (mHnsWalletService != null) mHnsWalletService.close();
        super.onDestroy();
    }

    @Override
    public boolean shouldStartGpuProcess() {
        return true;
    }

    @Override
    public void locked() {
        finish();
    }

    @Override
    public void backedUp() {}

    @Override
    public void onNewUnapprovedTx(TransactionInfo txInfo) {}

    @Override
    public void onUnapprovedTxUpdated(TransactionInfo txInfo) {}

    @Override
    public void onTransactionStatusChanged(TransactionInfo txInfo) {}
}
