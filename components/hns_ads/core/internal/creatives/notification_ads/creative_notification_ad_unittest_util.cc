/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"

#include "base/check.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"

namespace hns_ads {

CreativeNotificationAdList BuildCreativeNotificationAdsForTesting(
    const int count) {
  CHECK_GT(count, 0);

  CreativeNotificationAdList creative_ads;

  for (int i = 0; i < count; i++) {
    const CreativeNotificationAdInfo creative_ad =
        BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
    creative_ads.push_back(creative_ad);
  }

  return creative_ads;
}

CreativeNotificationAdInfo BuildCreativeNotificationAdForTesting(
    const bool should_use_random_uuids) {
  const CreativeAdInfo creative_ad =
      BuildCreativeAdForTesting(should_use_random_uuids);
  CreativeNotificationAdInfo creative_notification_ad(creative_ad);

  creative_notification_ad.title = "Test Ad Title";
  creative_notification_ad.body = "Test Ad Body";

  return creative_notification_ad;
}

}  // namespace hns_ads
