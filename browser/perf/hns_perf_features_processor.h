// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_PERF_HNS_PERF_FEATURES_PROCESSOR_H_
#define HNS_BROWSER_PERF_HNS_PERF_FEATURES_PROCESSOR_H_

class Profile;

namespace perf {

// A handler for --enable-hns-features-for-perf-testing switch.
void MaybeEnableHnsFeatureForPerfTesting(Profile* profile);

}  // namespace perf

#endif  // HNS_BROWSER_PERF_HNS_PERF_FEATURES_PROCESSOR_H_
