/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_NOTIFICATION_AD_NOTIFICATION_AD_HANDLER_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_NOTIFICATION_AD_NOTIFICATION_AD_HANDLER_UTIL_H_

#include <string>

namespace hns_ads {

struct NotificationAdInfo;

bool ShouldServe();

bool CanServeIfUserIsActive();

bool CanServeAtRegularIntervals();
bool ShouldServeAtRegularIntervals();

void ShowNotificationAd(const NotificationAdInfo& ad);
void CloseNotificationAd(const std::string& placement_id);
void DismissNotificationAd(const std::string& placement_id);
void NotificationAdTimedOut(const std::string& placement_id);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_NOTIFICATION_AD_NOTIFICATION_AD_HANDLER_UTIL_H_
