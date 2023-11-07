/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_FEDERATED_FEATURES_H_
#define HNS_COMPONENTS_HNS_FEDERATED_FEATURES_H_

#include "base/feature_list.h"

namespace hns_federated {

namespace features {

BASE_DECLARE_FEATURE(kFederatedLearning);

bool IsFederatedLearningEnabled();

// Operational Patterns
bool IsOperationalPatternsEnabled();
int GetCollectionIdLifetimeInSeconds();
int GetCollectionSlotSizeInSeconds();
int GetCollectionTimerIntervalInSeconds();
int GetMockTaskDurationInSeconds();
bool MockCollectionRequests();

}  // namespace features

}  // namespace hns_federated

#endif  // HNS_COMPONENTS_HNS_FEDERATED_FEATURES_H_
