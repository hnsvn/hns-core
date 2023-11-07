/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/settings/settings.h"

#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/public/feature/notification_ad_feature.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/ntp_background_images/common/pref_names.h"

namespace hns_ads {

bool UserHasJoinedHnsRewards() {
  return AdsClientHelper::GetInstance()->GetBooleanPref(
      hns_rewards::prefs::kEnabled);
}

bool UserHasOptedInToHnsNewsAds() {
  return AdsClientHelper::GetInstance()->GetBooleanPref(
             hns_news::prefs::kHnsNewsOptedIn) &&
         AdsClientHelper::GetInstance()->GetBooleanPref(
             hns_news::prefs::kNewTabPageShowToday);
}

bool UserHasOptedInToNewTabPageAds() {
  return AdsClientHelper::GetInstance()->GetBooleanPref(
             ntp_background_images::prefs::kNewTabPageShowBackgroundImage) &&
         AdsClientHelper::GetInstance()->GetBooleanPref(
             ntp_background_images::prefs::
                 kNewTabPageShowSponsoredImagesBackgroundImage);
}

bool UserHasOptedInToNotificationAds() {
  return UserHasJoinedHnsRewards() &&
         AdsClientHelper::GetInstance()->GetBooleanPref(
             prefs::kOptedInToNotificationAds);
}

int GetMaximumNotificationAdsPerHour() {
  const int ads_per_hour =
      static_cast<int>(AdsClientHelper::GetInstance()->GetInt64Pref(
          prefs::kMaximumNotificationAdsPerHour));

  return ads_per_hour > 0 ? ads_per_hour : kDefaultNotificationAdsPerHour.Get();
}

}  // namespace hns_ads
