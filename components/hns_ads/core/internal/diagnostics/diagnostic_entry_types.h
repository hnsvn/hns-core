/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_DIAGNOSTIC_ENTRY_TYPES_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_DIAGNOSTIC_ENTRY_TYPES_H_

namespace hns_ads {

// Diagnostic entries should be visually sorted based on the enum order.
enum class DiagnosticEntryType {
  kDeviceId,
  kOptedInToHnsNewsAds,
  kOptedInToNewTabPageAds,
  kOptedInToNotificationAds,
  kLocale,
  kCatalogId,
  kCatalogLastUpdated,
  kLastUnIdleTime
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_DIAGNOSTIC_ENTRY_TYPES_H_
