/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.app.domain;

import android.content.Context;

import org.chromium.hns_wallet.mojom.AssetRatioService;
import org.chromium.hns_wallet.mojom.BlockchainRegistry;
import org.chromium.hns_wallet.mojom.HnsWalletService;
import org.chromium.hns_wallet.mojom.EthTxManagerProxy;
import org.chromium.hns_wallet.mojom.JsonRpcService;
import org.chromium.hns_wallet.mojom.KeyringService;
import org.chromium.hns_wallet.mojom.NetworkInfo;
import org.chromium.hns_wallet.mojom.SolanaTxManagerProxy;
import org.chromium.hns_wallet.mojom.SwapService;
import org.chromium.hns_wallet.mojom.TxService;

// Under development, some parts not tested so use with caution
// A container for all the native services and APIs
public class WalletModel {
    private KeyringService mKeyringService;
    private BlockchainRegistry mBlockchainRegistry;
    private JsonRpcService mJsonRpcService;
    private TxService mTxService;
    private EthTxManagerProxy mEthTxManagerProxy;
    private SolanaTxManagerProxy mSolanaTxManagerProxy;
    private HnsWalletService mHnsWalletService;
    private AssetRatioService mAssetRatioService;
    private SwapService mSwapService;
    private final CryptoModel mCryptoModel;
    private final DappsModel mDappsModel;
    private final KeyringModel mKeyringModel;
    private final MarketModel mMarketModel;
    private Context mContext;
    private CryptoActions mCryptoActions;

    public WalletModel(Context context, KeyringService keyringService,
            BlockchainRegistry blockchainRegistry, JsonRpcService jsonRpcService,
            TxService txService, EthTxManagerProxy ethTxManagerProxy,
            SolanaTxManagerProxy solanaTxManagerProxy, AssetRatioService assetRatioService,
            HnsWalletService hnsWalletService, SwapService swapService) {
        mContext = context;
        mKeyringService = keyringService;
        mBlockchainRegistry = blockchainRegistry;
        mJsonRpcService = jsonRpcService;
        mTxService = txService;
        mEthTxManagerProxy = ethTxManagerProxy;
        mSolanaTxManagerProxy = solanaTxManagerProxy;
        mAssetRatioService = assetRatioService;
        mHnsWalletService = hnsWalletService;
        mSwapService = swapService;
        // Do not change the object initialisation order without discussion
        mCryptoActions = new CryptoActions();
        mCryptoModel = new CryptoModel(mContext, mTxService, mKeyringService, mBlockchainRegistry,
                mJsonRpcService, mEthTxManagerProxy, mSolanaTxManagerProxy, mHnsWalletService,
                mAssetRatioService, mCryptoActions, mSwapService);
        mDappsModel = new DappsModel(mJsonRpcService, mHnsWalletService, mKeyringService,
                mCryptoModel.getPendingTxHelper());
        mKeyringModel =
                new KeyringModel(mContext, mKeyringService, mHnsWalletService, mCryptoActions);
        mMarketModel = new MarketModel(mAssetRatioService);
        // be careful with dependencies, must avoid cycles
        mCryptoModel.setAccountInfosFromKeyRingModel(mKeyringModel.mAccountInfos);
        init();
    }

    public void resetServices(Context context, KeyringService keyringService,
            BlockchainRegistry blockchainRegistry, JsonRpcService jsonRpcService,
            TxService txService, EthTxManagerProxy ethTxManagerProxy,
            SolanaTxManagerProxy solanaTxManagerProxy, AssetRatioService assetRatioService,
            HnsWalletService hnsWalletService, SwapService swapService) {
        mContext = context;
        setKeyringService(keyringService);
        setBlockchainRegistry(blockchainRegistry);
        setJsonRpcService(jsonRpcService);
        setTxService(txService);
        setEthTxManagerProxy(ethTxManagerProxy);
        setSolanaTxManagerProxy(solanaTxManagerProxy);
        setAssetRatioService(assetRatioService);
        setHnsWalletService(hnsWalletService);
        mSwapService = swapService;
        mCryptoModel.resetServices(mContext, mTxService, mKeyringService, mBlockchainRegistry,
                mJsonRpcService, mEthTxManagerProxy, mSolanaTxManagerProxy, mHnsWalletService,
                mAssetRatioService);
        mDappsModel.resetServices(
                mJsonRpcService, mHnsWalletService, mCryptoModel.getPendingTxHelper());
        mKeyringModel.resetService(mContext, mKeyringService, hnsWalletService);
        mMarketModel.resetService(mAssetRatioService);
        init();
    }

    public void createAccountAndSetDefaultNetwork(NetworkInfo networkInfo) {
        getKeyringModel().addAccount(
                networkInfo.coin, networkInfo.chainId, null, isAccountAdded -> {
                    getNetworkModel().clearCreateAccountState();

                    if (isAccountAdded) {
                        getNetworkModel().setDefaultNetwork(networkInfo, success -> {});
                    }
                });
    }

    /*
     * Explicit method to ensure the safe initialisation to start the required data process
     */
    private void init() {
        mCryptoModel.init();
        mKeyringModel.init();
    }

    public KeyringModel getKeyringModel() {
        return mKeyringModel;
    }

    public boolean hasAllServices() {
        return getKeyringService() != null && getBlockchainRegistry() != null
                && getJsonRpcService() != null && getTxService() != null
                && getEthTxManagerProxy() != null && getSolanaTxManagerProxy() != null
                && getAssetRatioService() != null && getHnsWalletService() != null;
    }

    public CryptoModel getCryptoModel() {
        return mCryptoModel;
    }

    public NetworkModel getNetworkModel() {
        return mCryptoModel.getNetworkModel();
    }

    public DappsModel getDappsModel() {
        return mDappsModel;
    }

    public MarketModel getMarketModel() {
        return mMarketModel;
    }

    public KeyringService getKeyringService() {
        return mKeyringService;
    }

    public void setKeyringService(KeyringService mKeyringService) {
        this.mKeyringService = mKeyringService;
    }

    public BlockchainRegistry getBlockchainRegistry() {
        return mBlockchainRegistry;
    }

    public void setBlockchainRegistry(BlockchainRegistry mBlockchainRegistry) {
        this.mBlockchainRegistry = mBlockchainRegistry;
    }

    public JsonRpcService getJsonRpcService() {
        return mJsonRpcService;
    }

    public void setJsonRpcService(JsonRpcService mJsonRpcService) {
        this.mJsonRpcService = mJsonRpcService;
    }

    public TxService getTxService() {
        return mTxService;
    }

    public void setTxService(TxService mTxService) {
        this.mTxService = mTxService;
    }

    public EthTxManagerProxy getEthTxManagerProxy() {
        return mEthTxManagerProxy;
    }

    public void setEthTxManagerProxy(EthTxManagerProxy mEthTxManagerProxy) {
        this.mEthTxManagerProxy = mEthTxManagerProxy;
    }

    public SolanaTxManagerProxy getSolanaTxManagerProxy() {
        return mSolanaTxManagerProxy;
    }

    public void setSolanaTxManagerProxy(SolanaTxManagerProxy mSolanaTxManagerProxy) {
        this.mSolanaTxManagerProxy = mSolanaTxManagerProxy;
    }

    public HnsWalletService getHnsWalletService() {
        return mHnsWalletService;
    }

    public void setHnsWalletService(HnsWalletService mHnsWalletService) {
        this.mHnsWalletService = mHnsWalletService;
    }

    public AssetRatioService getAssetRatioService() {
        return mAssetRatioService;
    }

    public void setAssetRatioService(AssetRatioService mAssetRatioService) {
        this.mAssetRatioService = mAssetRatioService;
    }

    class CryptoActions implements CryptoSharedActions {
        @Override
        public void updateCoinType() {
            mCryptoModel.updateCoinType();
        }

        @Override
        public void onNewAccountAdded() {
            mCryptoModel.refreshTransactions();
        }
    }
}
