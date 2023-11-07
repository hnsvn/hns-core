/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_ENTRIES_LAST_UNIDLE_TIME_DIAGNOSTIC_ENTRY_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_ENTRIES_LAST_UNIDLE_TIME_DIAGNOSTIC_ENTRY_H_

#include <string>

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/diagnostics/entries/diagnostic_entry_interface.h"

namespace hns_ads {

class LastUnIdleTimeDiagnosticEntry final : public DiagnosticEntryInterface {
 public:
  LastUnIdleTimeDiagnosticEntry() = default;
  explicit LastUnIdleTimeDiagnosticEntry(base::Time last_unidle_at);

  // DiagnosticEntryInterface:
  DiagnosticEntryType GetType() const override;
  std::string GetName() const override;
  std::string GetValue() const override;

 private:
  base::Time last_unidle_at_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_ENTRIES_LAST_UNIDLE_TIME_DIAGNOSTIC_ENTRY_H_
