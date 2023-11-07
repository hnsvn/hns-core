// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/browser/perf/hns_perf_switches.h"

namespace perf::switches {

// Enables some Hns's widely used features for a testing profile in perf
// tests. --user-data-dir should be set.
const char kEnableHnsFeaturesForPerfTesting[] =
    "enable-hns-features-for-perf-testing";

}  // namespace perf::switches
