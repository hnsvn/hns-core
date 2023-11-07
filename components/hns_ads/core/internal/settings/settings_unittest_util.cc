/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_pref_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/ntp_background_images/common/pref_names.h"

namespace hns_ads {

void DisableHnsRewardsForTesting() {
  SetBooleanPref(hns_rewards::prefs::kEnabled, false);
}

void DisableHnsNewsAdsForTesting() {
  SetBooleanPref(hns_news::prefs::kHnsNewsOptedIn, false);
  SetBooleanPref(hns_news::prefs::kNewTabPageShowToday, false);
}

void DisableNewTabPageAdsForTesting() {
  SetBooleanPref(ntp_background_images::prefs::kNewTabPageShowBackgroundImage,
                 false);
  SetBooleanPref(ntp_background_images::prefs::
                     kNewTabPageShowSponsoredImagesBackgroundImage,
                 false);
}

void DisableNotificationAdsForTesting() {
  SetBooleanPref(prefs::kOptedInToNotificationAds, false);
}

void SetMaximumNotificationAdsPerHourForTesting(const int max_ads_per_hour) {
  SetInt64Pref(prefs::kMaximumNotificationAdsPerHour, max_ads_per_hour);
}

}  // namespace hns_ads
