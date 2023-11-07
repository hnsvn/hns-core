/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_FEDERATED_OPERATIONAL_PATTERNS_UTIL_H_
#define HNS_COMPONENTS_HNS_FEDERATED_OPERATIONAL_PATTERNS_UTIL_H_

#include <string>

#include "base/time/time.h"

namespace hns_federated {

int GetCollectionSlot();
std::string CreateCollectionId();

std::u16string FriendlyTime(const base::Time time);

std::string BuildCollectionPingPayload(const std::string collection_id,
                                       int slot);
std::string BuildDeletePingPayload(const std::string collection_id);

bool ShouldResetCollectionId(const std::string collection_id,
                             const base::Time expiration_time);

}  // namespace hns_federated

#endif  // HNS_COMPONENTS_HNS_FEDERATED_OPERATIONAL_PATTERNS_UTIL_H_
