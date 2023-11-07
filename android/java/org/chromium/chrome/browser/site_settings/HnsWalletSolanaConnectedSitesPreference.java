/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.site_settings;

import android.annotation.SuppressLint;
import android.content.Context;
import android.util.AttributeSet;

import androidx.preference.Preference;
import androidx.preference.PreferenceViewHolder;
import androidx.recyclerview.widget.RecyclerView;

import org.chromium.hns_wallet.mojom.HnsWalletService;
import org.chromium.hns_wallet.mojom.CoinType;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.crypto_wallet.HnsWalletServiceFactory;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;

public class HnsWalletSolanaConnectedSitesPreference
        extends Preference implements ConnectionErrorHandler,
                                      HnsWalletEthereumConnectedSitesListAdapter
                                              .HnsEthereumPermissionConnectedSitesDelegate {
    private RecyclerView mRecyclerView;
    private HnsWalletService mHnsWalletService;
    private HnsWalletEthereumConnectedSitesListAdapter mAdapter;

    public HnsWalletSolanaConnectedSitesPreference(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void onBindViewHolder(PreferenceViewHolder holder) {
        super.onBindViewHolder(holder);

        initHnsWalletService();

        mRecyclerView = (RecyclerView) holder.findViewById(R.id.connected_sites_list);
        updateWebSitestList();
    }

    public void destroy() {
        mHnsWalletService.close();
        mHnsWalletService = null;
    }

    @SuppressLint("NotifyDataSetChanged")
    private void updateWebSitestList() {
        mHnsWalletService.getWebSitesWithPermission(CoinType.SOL, webSites -> {
            if (mAdapter == null) {
                mAdapter = new HnsWalletEthereumConnectedSitesListAdapter(webSites, this);
                mRecyclerView.setAdapter(mAdapter);
            } else {
                mAdapter.setWebSites(webSites);
                mAdapter.notifyDataSetChanged();
            }
        });
    }

    @Override
    public void removePermission(String webSite) {
        mHnsWalletService.resetWebSitePermission(CoinType.SOL, webSite, success -> {
            if (success) {
                updateWebSitestList();
            }
        });
    }

    @Override
    public void onConnectionError(MojoException e) {
        mHnsWalletService.close();
        mHnsWalletService = null;
        initHnsWalletService();
    }

    private void initHnsWalletService() {
        if (mHnsWalletService != null) {
            return;
        }

        mHnsWalletService = HnsWalletServiceFactory.getInstance().getHnsWalletService(this);
    }
}
