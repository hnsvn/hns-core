/**
 * Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.notifications.channels;

import android.annotation.SuppressLint;
import android.app.NotificationManager;

import org.chromium.chrome.browser.notifications.R;
import org.chromium.components.browser_ui.notifications.channels.ChannelDefinitions;
import org.chromium.components.browser_ui.notifications.channels.ChannelDefinitions.PredefinedChannel;

import java.util.Map;
import java.util.Set;

public class HnsChannelDefinitions {
    public class ChannelId {
        public static final String HNS_ADS = "com.hns.browser.ads";
        public static final String HNS_ADS_BACKGROUND = "com.hns.browser.ads.background";
        public static final String HNS_BROWSER = "com.hns.browser";
    }

    public class ChannelGroupId {
        public static final String HNS_ADS = "com.hns.browser.ads";
        public static final String GENERAL = "general";
    }

    @SuppressLint("NewApi")
    static protected void addHnsChannels(
            Map<String, PredefinedChannel> map, Set<String> startup) {
        map.put(ChannelId.HNS_ADS,
                PredefinedChannel.create(ChannelId.HNS_ADS, R.string.hns_ads_text,
                        NotificationManager.IMPORTANCE_HIGH, ChannelGroupId.HNS_ADS));
        startup.add(ChannelId.HNS_ADS);

        map.put(ChannelId.HNS_ADS_BACKGROUND,
                PredefinedChannel.create(ChannelId.HNS_ADS_BACKGROUND,
                        R.string.notification_category_hns_ads_background,
                        NotificationManager.IMPORTANCE_LOW, ChannelGroupId.HNS_ADS));
        startup.add(ChannelId.HNS_ADS_BACKGROUND);
    }

    @SuppressLint("NewApi")
    static protected void addHnsChannelGroups(
            Map<String, ChannelDefinitions.PredefinedChannelGroup> map) {
        map.put(ChannelGroupId.HNS_ADS,
                new ChannelDefinitions.PredefinedChannelGroup(
                        ChannelGroupId.HNS_ADS, R.string.hns_ads_text));
    }
}
