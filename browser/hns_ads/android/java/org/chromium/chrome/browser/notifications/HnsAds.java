/**
 * Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.notifications;

import org.chromium.base.annotations.CalledByNative;
import org.chromium.chrome.browser.notifications.channels.HnsChannelDefinitions;

/**
 * This class provides the Hns Ads related methods for the native library
 * (hns/components/hns_ads/browser/notification_helper_android)
 */
public abstract class HnsAds {
    @CalledByNative
    public static String getHnsAdsChannelId() {
        return HnsChannelDefinitions.ChannelId.HNS_ADS;
    }

    @CalledByNative
    public static String getHnsAdsBackgroundChannelId() {
        return HnsChannelDefinitions.ChannelId.HNS_ADS_BACKGROUND;
    }
}
