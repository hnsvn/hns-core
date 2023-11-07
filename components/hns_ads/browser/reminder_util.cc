/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/browser/reminder_util.h"

#include "base/notreached.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_constants.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/grit/hns_generated_resources.h"
#include "url/gurl.h"

namespace hns_ads {

namespace {

constexpr char kReminderNotificationAdPlacementId[] =
    "e64373ac-2ca5-4f6b-b497-1f1d7ccd40c8";
constexpr char kReminderNotificationAdTargetUrl[] =
    "https://support.hns.com/hc/en-us/articles/14648356808845";

}  // namespace

namespace {

base::Value::Dict BuildClickedSameAdMultipleTimesReminder() {
  base::Value::Dict dict;

  dict.Set(kNotificationAdPlacementIdKey, kReminderNotificationAdPlacementId);
  dict.Set(
      kNotificationAdTitleKey,
      hns_l10n::GetLocalizedResourceUTF16String(
          IDS_HNS_ADS_NOTIFICATION_CLICKED_SAME_AD_MULTIPLE_TIMES_TITLE));
  dict.Set(kNotificationAdBodyKey,
           hns_l10n::GetLocalizedResourceUTF16String(
               IDS_HNS_ADS_NOTIFICATION_CLICKED_SAME_AD_MULTIPLE_TIMES_BODY));
  dict.Set(kNotificationAdTargetUrlKey, kReminderNotificationAdTargetUrl);

  return dict;
}

base::Value::Dict BuildExternalWalletConnectedReminder() {
  base::Value::Dict dict;

  dict.Set(kNotificationAdPlacementIdKey, kReminderNotificationAdPlacementId);
  dict.Set(kNotificationAdTitleKey,
           hns_l10n::GetLocalizedResourceUTF16String(
               IDS_HNS_ADS_NOTIFICATION_EXTERNAL_WALLET_CONNECTED_TITLE));
  dict.Set(kNotificationAdBodyKey,
           hns_l10n::GetLocalizedResourceUTF16String(
               IDS_HNS_ADS_NOTIFICATION_EXTERNAL_WALLET_CONNECTED_BODY));
  dict.Set(kNotificationAdTargetUrlKey, kReminderNotificationAdTargetUrl);

  return dict;
}

}  // namespace

base::Value::Dict BuildReminder(const mojom::ReminderType type) {
  switch (type) {
    case mojom::ReminderType::kClickedSameAdMultipleTimes: {
      return BuildClickedSameAdMultipleTimesReminder();
    }

    case mojom::ReminderType::kExternalWalletConnected: {
      return BuildExternalWalletConnectedReminder();
    }
  }

  NOTREACHED_NORETURN() << "Unexpected value for mojom::ReminderType: " << type;
}

bool IsReminder(const std::string& placement_id) {
  return placement_id == kReminderNotificationAdPlacementId;
}

GURL GetReminderTargetUrl() {
  return GURL(kReminderNotificationAdTargetUrl);
}

}  // namespace hns_ads
