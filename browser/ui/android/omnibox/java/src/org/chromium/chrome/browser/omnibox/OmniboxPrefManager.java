/**
 * Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.omnibox;

import android.content.SharedPreferences;

import org.chromium.base.ContextUtils;

import java.util.Calendar;
import java.util.Date;

public class OmniboxPrefManager {
    private static final String HNS_SEARCH_PROMO_BANNER_EXPIRED_DATE =
            "hns_search_promo_banner_expired_date";
    private static final String HNS_SEARCH_PROMO_BANNER_MAYBE_LATER =
            "hns_search_promo_banner_maybe_later";
    private static final String HNS_SEARCH_PROMO_BANNER_DISMISSED =
            "hns_search_promo_banner_dismissed";

    private static OmniboxPrefManager sInstance;
    private final SharedPreferences mSharedPreferences;

    private boolean isHnsSearchPromoBannerDismissedCurrentSession;

    private OmniboxPrefManager() {
        mSharedPreferences = ContextUtils.getAppSharedPreferences();
    }

    /**
     * Returns the singleton instance of OmniboxPrefManager, creating it if needed.
     */
    public static OmniboxPrefManager getInstance() {
        if (sInstance == null) {
            sInstance = new OmniboxPrefManager();
        }
        return sInstance;
    }

    public long getHnsSearchPromoBannerExpiredDate() {
        return mSharedPreferences.getLong(HNS_SEARCH_PROMO_BANNER_EXPIRED_DATE, 0);
    }

    public void setHnsSearchPromoBannerExpiredDate() {
        Calendar calender = Calendar.getInstance();
        calender.setTime(new Date());
        calender.add(Calendar.DATE, 14);

        SharedPreferences.Editor sharedPreferencesEditor = mSharedPreferences.edit();
        sharedPreferencesEditor.putLong(
                HNS_SEARCH_PROMO_BANNER_EXPIRED_DATE, calender.getTimeInMillis());
        sharedPreferencesEditor.apply();
    }

    public boolean isHnsSearchPromoBannerMaybeLater() {
        return mSharedPreferences.getBoolean(HNS_SEARCH_PROMO_BANNER_MAYBE_LATER, false);
    }

    public void setHnsSearchPromoBannerMaybeLater() {
        isHnsSearchPromoBannerDismissedCurrentSession = true;

        SharedPreferences.Editor sharedPreferencesEditor = mSharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(HNS_SEARCH_PROMO_BANNER_MAYBE_LATER, true);
        sharedPreferencesEditor.apply();
    }

    public boolean isHnsSearchPromoBannerDismissed() {
        return mSharedPreferences.getBoolean(HNS_SEARCH_PROMO_BANNER_DISMISSED, false);
    }

    public void setHnsSearchPromoBannerDismissed() {
        SharedPreferences.Editor sharedPreferencesEditor = mSharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(HNS_SEARCH_PROMO_BANNER_DISMISSED, true);
        sharedPreferencesEditor.apply();
    }

    public boolean isHnsSearchPromoBannerDismissedCurrentSession() {
        return isHnsSearchPromoBannerDismissedCurrentSession;
    }
}
