/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NOTIFICATION_ADS_CREATIVE_NOTIFICATION_ADS_DATABASE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NOTIFICATION_ADS_CREATIVE_NOTIFICATION_ADS_DATABASE_UTIL_H_

#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"

namespace hns_ads::database {

void DeleteCreativeNotificationAds();

void SaveCreativeNotificationAds(
    const CreativeNotificationAdList& creative_ads);

}  // namespace hns_ads::database

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NOTIFICATION_ADS_CREATIVE_NOTIFICATION_ADS_DATABASE_UTIL_H_
