/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import static org.chromium.ui.base.ViewUtils.dpToPx;

import android.app.Activity;
import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import androidx.appcompat.widget.AppCompatImageView;
import androidx.cardview.widget.CardView;
import androidx.preference.Preference;
import androidx.preference.PreferenceViewHolder;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.InternetConnection;
import org.chromium.chrome.browser.util.ConfigurationUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.HnsVpnUtils;
import org.chromium.ui.base.DeviceFormFactor;
import org.chromium.ui.widget.Toast;

public class VpnCalloutPreference extends Preference {
    public VpnCalloutPreference(Context context) {
        super(context);
        setLayoutResource(R.layout.vpn_settings_callout_modal_layout);
    }

    @Override
    public void onBindViewHolder(PreferenceViewHolder holder) {
        super.onBindViewHolder(holder);

        View mainView = holder.findViewById(R.id.callout_cardview);

        boolean isTablet = DeviceFormFactor.isNonMultiDisplayContextOnTablet(getContext());

        int width = (int) ((getContext().getResources().getDisplayMetrics().widthPixels) * 1.00);
        if (isTablet || ConfigurationUtils.isLandscape(getContext())) {
            width = (int) dpToPx(getContext(), 390);
        }
        int height = CardView.LayoutParams.WRAP_CONTENT;
        ViewGroup.LayoutParams params = mainView.getLayoutParams();
        params.width = width;
        params.height = height;
        mainView.setLayoutParams(params);
        mainView.requestLayout();

        AppCompatImageView btnClose = (AppCompatImageView) holder.findViewById(R.id.modal_close);
        btnClose.setOnClickListener(v -> {
            getPreferenceManager().getPreferenceScreen().removePreference(this);
            HnsVpnPrefUtils.setCallout(false);
        });
        Button btnLearnMore = (Button) holder.findViewById(R.id.btn_learn_more);
        btnLearnMore.setOnClickListener(v -> {
            if (!InternetConnection.isNetworkAvailable(getContext())) {
                Toast.makeText(getContext(), R.string.no_internet, Toast.LENGTH_SHORT).show();
            } else {
                HnsVpnUtils.openHnsVpnPlansActivity((Activity) getContext());
                HnsVpnPrefUtils.setCallout(false);
            }
        });
    }
}
