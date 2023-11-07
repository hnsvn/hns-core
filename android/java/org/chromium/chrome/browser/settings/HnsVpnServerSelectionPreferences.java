/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.graphics.Rect;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.vpn.HnsVpnObserver;
import org.chromium.chrome.browser.vpn.adapters.HnsVpnServerSelectionAdapter;
import org.chromium.chrome.browser.vpn.models.HnsVpnServerRegion;
import org.chromium.chrome.browser.vpn.utils.HnsVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnUtils;
import org.chromium.ui.widget.Toast;

import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class HnsVpnServerSelectionPreferences
        extends HnsPreferenceFragment implements HnsVpnObserver {
    private HnsVpnServerSelectionAdapter mHnsVpnServerSelectionAdapter;
    private LinearLayout mServerSelectionListLayout;
    private ProgressBar mServerSelectionProgress;

    public interface OnServerRegionSelection {
        void onServerRegionClick(HnsVpnServerRegion vpnServerRegion);
    }
    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.hns_vpn_server_selection_layout, container, false);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        getActivity().setTitle(R.string.change_location);

        TextView automaticText = (TextView) getView().findViewById(R.id.automatic_server_text);
        automaticText.setText(getActivity().getResources().getString(R.string.automatic));
        if (HnsVpnPrefUtils.getServerRegion().equals(
                    HnsVpnPrefUtils.PREF_HNS_VPN_AUTOMATIC)) {
            automaticText.setCompoundDrawablesWithIntrinsicBounds(
                    0, 0, R.drawable.ic_server_selection_check, 0);
        } else {
            automaticText.setCompoundDrawablesWithIntrinsicBounds(0, 0, 0, 0);
        }
        automaticText.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                HnsVpnUtils.selectedServerRegion = HnsVpnPrefUtils.PREF_HNS_VPN_AUTOMATIC;
                HnsVpnUtils.mIsServerLocationChanged = true;
                getActivity().onBackPressed();
            }
        });

        mServerSelectionListLayout =
                (LinearLayout) getView().findViewById(R.id.server_selection_list_layout);
        mServerSelectionProgress =
                (ProgressBar) getView().findViewById(R.id.server_selection_progress);

        LinearLayoutManager linearLayoutManager = new LinearLayoutManager(getContext());
        RecyclerView serverRegionList =
                (RecyclerView) getView().findViewById(R.id.server_selection_list);
        serverRegionList.addItemDecoration(
                new DividerItemDecoration(getActivity(), linearLayoutManager.getOrientation()) {
                    @Override
                    public void getItemOffsets(Rect outRect, View view, RecyclerView parent,
                            RecyclerView.State state) {
                        int position = parent.getChildAdapterPosition(view);
                        // hide the divider for the last child
                        if (position == state.getItemCount() - 1) {
                            outRect.setEmpty();
                        } else {
                            super.getItemOffsets(outRect, view, parent, state);
                        }
                    }
                });
        List<HnsVpnServerRegion> hnsVpnServerRegions =
                HnsVpnUtils.getServerLocations(HnsVpnPrefUtils.getServerRegions());
        Collections.sort(hnsVpnServerRegions, new Comparator<HnsVpnServerRegion>() {
            @Override
            public int compare(HnsVpnServerRegion hnsVpnServerRegion1,
                    HnsVpnServerRegion hnsVpnServerRegion2) {
                return hnsVpnServerRegion1.getNamePretty().compareToIgnoreCase(
                        hnsVpnServerRegion2.getNamePretty());
            }
        });
        mHnsVpnServerSelectionAdapter = new HnsVpnServerSelectionAdapter();
        mHnsVpnServerSelectionAdapter.setVpnServerRegions(hnsVpnServerRegions);
        mHnsVpnServerSelectionAdapter.setOnServerRegionSelection(onServerRegionSelection);
        serverRegionList.setAdapter(mHnsVpnServerSelectionAdapter);
        serverRegionList.setLayoutManager(linearLayoutManager);

        super.onActivityCreated(savedInstanceState);
    }

    OnServerRegionSelection onServerRegionSelection = new OnServerRegionSelection() {
        @Override
        public void onServerRegionClick(HnsVpnServerRegion hnsVpnServerRegion) {
            if (HnsVpnPrefUtils.getServerRegion().equals(hnsVpnServerRegion.getName())) {
                Toast.makeText(getActivity(), R.string.already_selected_the_server,
                             Toast.LENGTH_SHORT)
                        .show();
            } else {
                HnsVpnUtils.selectedServerRegion = hnsVpnServerRegion.getName();
                HnsVpnUtils.mIsServerLocationChanged = true;
                getActivity().onBackPressed();
            }
        }
    };

    public void showProgress() {
        if (mServerSelectionProgress != null) {
            mServerSelectionProgress.setVisibility(View.VISIBLE);
        }
        if (mServerSelectionListLayout != null) {
            mServerSelectionListLayout.setAlpha(0.4f);
        }
    }

    public void hideProgress() {
        if (mServerSelectionProgress != null) {
            mServerSelectionProgress.setVisibility(View.GONE);
        }
        if (mServerSelectionListLayout != null) {
            mServerSelectionListLayout.setAlpha(1f);
        }
    }
}
