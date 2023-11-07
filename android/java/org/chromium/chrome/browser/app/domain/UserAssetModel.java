/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.app.domain;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import org.chromium.hns_wallet.mojom.BlockchainRegistry;
import org.chromium.hns_wallet.mojom.BlockchainToken;
import org.chromium.hns_wallet.mojom.HnsWalletConstants;
import org.chromium.hns_wallet.mojom.HnsWalletService;
import org.chromium.hns_wallet.mojom.JsonRpcService;
import org.chromium.hns_wallet.mojom.NetworkInfo;
import org.chromium.chrome.browser.crypto_wallet.adapters.WalletCoinAdapter;
import org.chromium.chrome.browser.crypto_wallet.util.AndroidUtils;
import org.chromium.chrome.browser.crypto_wallet.util.AssetUtils;
import org.chromium.chrome.browser.crypto_wallet.util.JavaUtils;
import org.chromium.chrome.browser.crypto_wallet.util.NetworkUtils;
import org.chromium.chrome.browser.crypto_wallet.util.TokenUtils;
import org.chromium.chrome.browser.crypto_wallet.util.WalletConstants;

import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.stream.Stream;

public class UserAssetModel {
    private final Object mLock = new Object();
    private final HnsWalletService mHnsWalletService;
    private final JsonRpcService mJsonRpcService;
    private final BlockchainRegistry mBlockchainRegistry;
    private final WalletCoinAdapter.AdapterType mType;
    private NetworkInfo mSelectedNetwork;
    private List<NetworkInfo> mCryptoNetworks;
    private final CryptoSharedData mSharedData;
    private final MutableLiveData<AssetsResult> _mAssetsResult;
    public LiveData<AssetsResult> mAssetsResult;

    public UserAssetModel(HnsWalletService hnsWalletService, JsonRpcService jsonRpcService,
            BlockchainRegistry blockchainRegistry, CryptoSharedData sharedData,
            WalletCoinAdapter.AdapterType type) {
        mHnsWalletService = hnsWalletService;
        mJsonRpcService = jsonRpcService;
        mBlockchainRegistry = blockchainRegistry;
        mSharedData = sharedData;
        mType = type;
        _mAssetsResult = new MutableLiveData<>();
        mAssetsResult = _mAssetsResult;
    }

    public void fetchAssets(boolean nftsOnly, NetworkInfo selectedNetwork) {
        synchronized (mLock) {
            mSelectedNetwork = selectedNetwork;
            if (JavaUtils.anyNull(mHnsWalletService, mJsonRpcService, mSelectedNetwork)) return;
            NetworkModel.getAllNetworks(
                    mJsonRpcService, mSharedData.getSupportedCryptoCoins(), allNetworks -> {
                        mCryptoNetworks = allNetworks;
                        if (mType == WalletCoinAdapter.AdapterType.EDIT_VISIBLE_ASSETS_LIST) {
                            if (NetworkUtils.isAllNetwork(mSelectedNetwork)) {
                                fetchAllNetworksAssets(nftsOnly);
                            } else {
                                TokenUtils.getVisibleUserAssetsFiltered(mHnsWalletService,
                                        mSelectedNetwork, mSelectedNetwork.coin,
                                        TokenUtils.TokenType.ALL,
                                        userAssets
                                        -> TokenUtils.getAllTokensFiltered(mHnsWalletService,
                                                mBlockchainRegistry, mSelectedNetwork,
                                                nftsOnly ? TokenUtils.TokenType.NFTS
                                                         : TokenUtils.TokenType.NON_NFTS,
                                                tokens -> {
                                                    _mAssetsResult.postValue(
                                                            new AssetsResult(Arrays.asList(tokens),
                                                                    Arrays.asList(userAssets)));
                                                }));
                            }
                        }
                    });
        }
    }

    private void fetchAllNetworksAssets(boolean nftsOnly) {
        mHnsWalletService.getAllUserAssets(userAssets -> {
            Stream<BlockchainToken> supportedNetworkAssets =
                    Arrays.stream(userAssets)
                            .filter(token
                                    -> !WalletConstants.UNSUPPORTED_NETWORKS.contains(
                                            token.chainId))
                            .filter(token -> nftsOnly == token.isNft);
            if (!AndroidUtils.isDebugBuild()) {
                supportedNetworkAssets = supportedNetworkAssets.filter(
                        token -> !token.chainId.equals(HnsWalletConstants.LOCALHOST_CHAIN_ID));
            }
            BlockchainToken[] supportedNetworkAssetsArray =
                    supportedNetworkAssets.toArray(BlockchainToken[] ::new);

            TokenUtils.getAllTokensFiltered(mBlockchainRegistry, mCryptoNetworks,
                    nftsOnly ? TokenUtils.TokenType.NFTS : TokenUtils.TokenType.NON_NFTS,
                    tokens -> {
                        sortByNetwork(tokens);
                        var filteredTokens = TokenUtils.distinctiveConcatenatedArrays(
                                tokens, supportedNetworkAssetsArray);
                        _mAssetsResult.postValue(new AssetsResult(Arrays.asList(filteredTokens),
                                Arrays.asList(supportedNetworkAssetsArray)));
                    });
        });
    }

    private void sortByNetwork(BlockchainToken[] tokens) {
        var networkIndexMap = AssetUtils.toNetworkIndexMap(mCryptoNetworks);
        Arrays.sort(tokens, Comparator.comparing(token -> networkIndexMap.get(token.chainId)));
    }

    public static class AssetsResult {
        public List<BlockchainToken> tokens;
        public List<BlockchainToken> userAssets;

        public AssetsResult(List<BlockchainToken> tokens, List<BlockchainToken> userAssets) {
            this.tokens = tokens;
            this.userAssets = userAssets;
        }
    }
}
