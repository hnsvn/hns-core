/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_NET_BASE_FEATURES_H_
#define HNS_CHROMIUM_SRC_NET_BASE_FEATURES_H_

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"
#include "net/base/net_export.h"

namespace net {
namespace features {

NET_EXPORT BASE_DECLARE_FEATURE(kHnsEphemeralStorage);
NET_EXPORT BASE_DECLARE_FEATURE(kHnsEphemeralStorageKeepAlive);
NET_EXPORT extern const base::FeatureParam<int>
    kHnsEphemeralStorageKeepAliveTimeInSeconds;
NET_EXPORT BASE_DECLARE_FEATURE(kHnsFirstPartyEphemeralStorage);
NET_EXPORT BASE_DECLARE_FEATURE(kHnsHttpsByDefault);
NET_EXPORT BASE_DECLARE_FEATURE(kHnsPartitionBlobStorage);
NET_EXPORT BASE_DECLARE_FEATURE(kHnsPartitionHSTS);
NET_EXPORT BASE_DECLARE_FEATURE(kHnsTorWindowsHttpsOnly);
NET_EXPORT BASE_DECLARE_FEATURE(kHnsForgetFirstPartyStorage);
NET_EXPORT extern const base::FeatureParam<int>
    kHnsForgetFirstPartyStorageStartupCleanupDelayInSeconds;
NET_EXPORT extern const base::FeatureParam<bool>
    kHnsForgetFirstPartyStorageByDefault;

}  // namespace features
}  // namespace net

#include "src/net/base/features.h"  // IWYU pragma: export

#endif  // HNS_CHROMIUM_SRC_NET_BASE_FEATURES_H_
