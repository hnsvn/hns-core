/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.playlist.settings;

import android.content.Context;
import android.content.res.Resources;
import android.util.AttributeSet;
import android.widget.TextView;

import androidx.preference.Preference;
import androidx.preference.PreferenceViewHolder;

import org.chromium.base.ApiCompatibilityUtils;
import org.chromium.chrome.R;

/**
 * The preference used to reset Hns Playlist.
 */
public class HnsPlaylistResetPreference
        extends Preference implements Preference.OnPreferenceClickListener {
    private String TAG = "HnsPlaylistResetPreference";

    private int mPrefAccentColor;
    private final String mConfirmationPhrase;

    /**
     * Constructor for HnsPlaylistResetPreference.
     */
    public HnsPlaylistResetPreference(Context context, AttributeSet attrs) {
        super(context, attrs);

        Resources resources = getContext().getResources();
        mPrefAccentColor =
                ApiCompatibilityUtils.getColor(resources, R.color.wallet_error_text_color);
        mConfirmationPhrase =
                resources.getString(R.string.hns_wallet_reset_settings_confirmation_phrase);
        setOnPreferenceClickListener(this);
    }

    @Override
    public void onBindViewHolder(PreferenceViewHolder holder) {
        super.onBindViewHolder(holder);
        TextView titleView = (TextView) holder.findViewById(android.R.id.title);
        titleView.setTextColor(mPrefAccentColor);
    }

    @Override
    public boolean onPreferenceClick(Preference preference) {
        return true;
    }
}
