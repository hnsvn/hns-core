/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "src/net/base/features.cc"

#include "base/feature_override.h"

namespace net {
namespace features {

OVERRIDE_FEATURE_DEFAULT_STATES({{
    {kEnableWebTransportDraft07, base::FEATURE_DISABLED_BY_DEFAULT},
    {kNoncedPartitionedCookies, base::FEATURE_DISABLED_BY_DEFAULT},
    // Enable NIK-partitioning by default.
    {kPartitionConnectionsByNetworkIsolationKey,
     base::FEATURE_ENABLED_BY_DEFAULT},
    {kPartitionedCookies, base::FEATURE_DISABLED_BY_DEFAULT},
    {kPartitionHttpServerPropertiesByNetworkIsolationKey,
     base::FEATURE_ENABLED_BY_DEFAULT},
    {kPartitionSSLSessionsByNetworkIsolationKey,
     base::FEATURE_ENABLED_BY_DEFAULT},
    {kSplitHostCacheByNetworkIsolationKey, base::FEATURE_ENABLED_BY_DEFAULT},
    // It is necessary yet to make chromium storage partitioning compatible with
    // Hns ephemeral storage. For reference:
    // https://github.com/hnsvn/hns-browser/issues/26165
    {kSupportPartitionedBlobUrl, base::FEATURE_DISABLED_BY_DEFAULT},
    {kThirdPartyPartitionedStorageAllowedByDefault,
     base::FEATURE_DISABLED_BY_DEFAULT},
}});

BASE_FEATURE(kHnsEphemeralStorage,
             "EphemeralStorage",
             base::FEATURE_ENABLED_BY_DEFAULT);
BASE_FEATURE(kHnsEphemeralStorageKeepAlive,
             "HnsEphemeralStorageKeepAlive",
             base::FEATURE_ENABLED_BY_DEFAULT);

const base::FeatureParam<int> kHnsEphemeralStorageKeepAliveTimeInSeconds = {
    &kHnsEphemeralStorageKeepAlive,
    "HnsEphemeralStorageKeepAliveTimeInSeconds", 30};

BASE_FEATURE(kHnsFirstPartyEphemeralStorage,
             "HnsFirstPartyEphemeralStorage",
             base::FEATURE_ENABLED_BY_DEFAULT);

// Partition Blob storage in ephemeral context.
BASE_FEATURE(kHnsPartitionBlobStorage,
             "HnsPartitionBlobStorage",
             base::FEATURE_ENABLED_BY_DEFAULT);

// Partition HSTS state storage by top frame site.
BASE_FEATURE(kHnsPartitionHSTS,
             "HnsPartitionHSTS",
             base::FEATURE_ENABLED_BY_DEFAULT);

// Enables HTTPS-Only Mode in Private Windows with Tor by default.
BASE_FEATURE(kHnsTorWindowsHttpsOnly,
             "HnsTorWindowsHttpsOnly",
             base::FEATURE_ENABLED_BY_DEFAULT);

// Enabled HTTPS by Default.
BASE_FEATURE(kHnsHttpsByDefault,
             "HttpsByDefault",
             base::FEATURE_ENABLED_BY_DEFAULT);

// Add "Forget by default" cookie blocking mode which cleanups storage after a
// website is closed.
BASE_FEATURE(kHnsForgetFirstPartyStorage,
             "HnsForgetFirstPartyStorage",
             base::FEATURE_DISABLED_BY_DEFAULT);

const base::FeatureParam<int>
    kHnsForgetFirstPartyStorageStartupCleanupDelayInSeconds = {
        &kHnsForgetFirstPartyStorage,
        "HnsForgetFirstPartyStorageStartupCleanupDelayInSeconds", 5};

const base::FeatureParam<bool> kHnsForgetFirstPartyStorageByDefault = {
    &kHnsForgetFirstPartyStorage, "HnsForgetFirstPartyStorageByDefault",
    false};

}  // namespace features
}  // namespace net
