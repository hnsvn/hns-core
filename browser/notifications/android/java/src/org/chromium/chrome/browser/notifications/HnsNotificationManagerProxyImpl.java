/**
 * Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.notifications;

import android.annotation.TargetApi;
import android.app.NotificationChannel;
import android.content.Context;
import android.os.Build;

import org.chromium.chrome.browser.notifications.channels.HnsChannelDefinitions;
import org.chromium.components.browser_ui.notifications.NotificationManagerProxyImpl;

public class HnsNotificationManagerProxyImpl extends NotificationManagerProxyImpl {
    public HnsNotificationManagerProxyImpl(Context context) {
        super(context);
    }

    @TargetApi(Build.VERSION_CODES.O)
    @Override
    public void createNotificationChannel(NotificationChannel channel) {
        if (channel.getId().equals(HnsChannelDefinitions.ChannelId.HNS_ADS)) {
            channel.setVibrationPattern(new long[] {0L});
            channel.enableVibration(true);
        }
        super.createNotificationChannel(channel);
    }
}
