/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/diagnostics/entries/last_unidle_time_diagnostic_util.h"

#include <memory>
#include <utility>

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_manager.h"
#include "hns/components/hns_ads/core/internal/diagnostics/entries/last_unidle_time_diagnostic_entry.h"

namespace hns_ads {

void SetLastUnIdleTimeDiagnosticEntry(const base::Time last_unidle_at) {
  auto last_unidle_time_diagnostic_entry =
      std::make_unique<LastUnIdleTimeDiagnosticEntry>(last_unidle_at);

  DiagnosticManager::GetInstance().SetEntry(
      std::move(last_unidle_time_diagnostic_entry));
}

}  // namespace hns_ads
