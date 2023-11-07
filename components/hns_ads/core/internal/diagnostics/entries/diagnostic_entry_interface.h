/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_ENTRIES_DIAGNOSTIC_ENTRY_INTERFACE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_ENTRIES_DIAGNOSTIC_ENTRY_INTERFACE_H_

#include <string>

#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_entry_types.h"

namespace hns_ads {

class DiagnosticEntryInterface {
 public:
  virtual ~DiagnosticEntryInterface() = default;

  virtual DiagnosticEntryType GetType() const = 0;
  virtual std::string GetName() const = 0;
  virtual std::string GetValue() const = 0;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_ENTRIES_DIAGNOSTIC_ENTRY_INTERFACE_H_
