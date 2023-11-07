/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_ENTRIES_LAST_UNIDLE_TIME_DIAGNOSTIC_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_ENTRIES_LAST_UNIDLE_TIME_DIAGNOSTIC_UTIL_H_

namespace base {
class Time;
}  // namespace base

namespace hns_ads {

void SetLastUnIdleTimeDiagnosticEntry(base::Time last_unidle_at);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_ENTRIES_LAST_UNIDLE_TIME_DIAGNOSTIC_UTIL_H_
