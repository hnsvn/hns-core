/**
 * Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.vpn.fragments;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;

import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.HnsDialogFragment;
import org.chromium.chrome.browser.app.HnsActivity;
import org.chromium.chrome.browser.vpn.utils.HnsVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnUtils;

public class LinkVpnSubscriptionDialogFragment
        extends HnsDialogFragment implements View.OnClickListener {
    private static final String TAG = "LinkVpnSubscription";

    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_link_subscription_dialog, container, false);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        HnsVpnPrefUtils.setLinkSubscriptionDialogShown(true);
        Button mDoneButton = view.findViewById(R.id.btn_done);
        mDoneButton.setOnClickListener(this);
        ImageView btnClose = view.findViewById(R.id.modal_close);
        btnClose.setOnClickListener(this);
    }

    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.btn_done) {
            try {
                HnsActivity hnsActivity = HnsActivity.getHnsActivity();
                hnsActivity.openNewOrSelectExistingTab(HnsVpnUtils.getHnsAccountUrl());
            } catch (HnsActivity.HnsActivityNotFoundException e) {
                Log.e(TAG, "onClick btn_done " + e);
            }
        }
        dismiss();
    }
}
