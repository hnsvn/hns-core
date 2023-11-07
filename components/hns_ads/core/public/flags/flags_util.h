/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_FLAGS_FLAGS_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_FLAGS_FLAGS_UTIL_H_

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

namespace hns_ads {

// Builds Flags basing on command line arguments and environment.
mojom::FlagsPtr BuildFlags();

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_FLAGS_FLAGS_UTIL_H_
