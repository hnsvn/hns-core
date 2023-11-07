// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_FEDERATED_NOTIFICATION_AD_TASK_CONSTANTS_H_
#define HNS_COMPONENTS_HNS_FEDERATED_NOTIFICATION_AD_TASK_CONSTANTS_H_

namespace hns_federated {

constexpr char kNotificationAdTaskName[] = "notification_ad_timing_task";
constexpr int kNotificationAdTaskId = 0;

constexpr int kMaxEvents = 200;
constexpr int kFeaturesPerEvent = 30;
constexpr int kMaxNumberOfRecords = kMaxEvents * kFeaturesPerEvent;
constexpr base::TimeDelta kMaxRetentionDays = base::Days(30);

}  // namespace hns_federated

#endif  // HNS_COMPONENTS_HNS_FEDERATED_NOTIFICATION_AD_TASK_CONSTANTS_H_
