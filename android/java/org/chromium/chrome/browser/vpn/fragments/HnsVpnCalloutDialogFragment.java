/**
 * Copyright (c) 2021 The Hns Authors. All rights reserved.
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

import org.chromium.chrome.R;
import org.chromium.chrome.browser.HnsDialogFragment;
import org.chromium.chrome.browser.InternetConnection;
import org.chromium.chrome.browser.vpn.utils.HnsVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnUtils;

public class HnsVpnCalloutDialogFragment
        extends HnsDialogFragment implements View.OnClickListener {
    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_vpn_callout_dialog, container, false);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        Button mEnableButton = view.findViewById(R.id.btn_enable);
        mEnableButton.setOnClickListener(this);

        ImageView btnClose = view.findViewById(R.id.modal_close);
        btnClose.setOnClickListener(this);

        HnsVpnPrefUtils.setCallout(false);
    }

    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.btn_enable) {
            if (!InternetConnection.isNetworkAvailable(getActivity())) {
                HnsVpnUtils.showToast(getResources().getString(R.string.no_internet));
            } else {
                HnsVpnUtils.openHnsVpnPlansActivity(getActivity());
            }
        }
        dismiss();
    }
}
