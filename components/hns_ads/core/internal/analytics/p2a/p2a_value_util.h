/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ANALYTICS_P2A_P2A_VALUE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ANALYTICS_P2A_P2A_VALUE_UTIL_H_

#include <string>
#include <vector>

#include "base/values.h"

namespace hns_ads::p2a {

base::Value::List EventsToValue(const std::vector<std::string>& events);

}  // namespace hns_ads::p2a

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ANALYTICS_P2A_P2A_VALUE_UTIL_H_
