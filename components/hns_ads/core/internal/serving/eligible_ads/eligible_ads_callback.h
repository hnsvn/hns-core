/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_ELIGIBLE_ADS_CALLBACK_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_ELIGIBLE_ADS_CALLBACK_H_

#include "base/functional/callback.h"

namespace hns_ads {

template <typename T>
using EligibleAdsCallback =
    base::OnceCallback<void(bool success, const T& eligible_ads)>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_ELIGIBLE_ADS_ELIGIBLE_ADS_CALLBACK_H_
