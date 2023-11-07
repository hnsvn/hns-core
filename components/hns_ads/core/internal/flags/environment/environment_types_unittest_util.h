/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FLAGS_ENVIRONMENT_ENVIRONMENT_TYPES_UNITTEST_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FLAGS_ENVIRONMENT_ENVIRONMENT_TYPES_UNITTEST_UTIL_H_

#include <string>

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"

namespace hns_ads {

std::string EnvironmentTypeEnumToStringForTesting(
    mojom::EnvironmentType environment_type);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FLAGS_ENVIRONMENT_ENVIRONMENT_TYPES_UNITTEST_UTIL_H_
