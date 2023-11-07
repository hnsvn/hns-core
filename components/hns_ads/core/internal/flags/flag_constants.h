/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FLAGS_FLAG_CONSTANTS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FLAGS_FLAG_CONSTANTS_H_

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"
#include "build/build_config.h"

namespace hns_ads {

#if defined(OFFICIAL_BUILD)
constexpr mojom::EnvironmentType kDefaultEnvironmentType =
    mojom::EnvironmentType::kProduction;
#else   // OFFICIAL_BUILD
constexpr mojom::EnvironmentType kDefaultEnvironmentType =
    mojom::EnvironmentType::kStaging;
#endif  // !OFFICIAL_BUILD

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FLAGS_FLAG_CONSTANTS_H_
