/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_REMINDER_REMINDERS_CLICKED_SAME_AD_MULTIPLE_TIMES_REMINDER_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_REMINDER_REMINDERS_CLICKED_SAME_AD_MULTIPLE_TIMES_REMINDER_UTIL_H_

namespace hns_ads {

struct HistoryItemInfo;

bool DidUserClickTheSameAdMultipleTimes(const HistoryItemInfo& history_item);

void RemindUserTheyDoNotNeedToClickToEarnRewards();

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_REMINDER_REMINDERS_CLICKED_SAME_AD_MULTIPLE_TIMES_REMINDER_UTIL_H_
