/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_manager.h"

#include <utility>

#include "base/check.h"
#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_value_util.h"
#include "hns/components/hns_ads/core/internal/diagnostics/entries/catalog_id_diagnostic_entry.h"
#include "hns/components/hns_ads/core/internal/diagnostics/entries/catalog_last_updated_diagnostic_entry.h"
#include "hns/components/hns_ads/core/internal/diagnostics/entries/device_id_diagnostic_entry.h"
#include "hns/components/hns_ads/core/internal/diagnostics/entries/last_unidle_time_diagnostic_entry.h"
#include "hns/components/hns_ads/core/internal/diagnostics/entries/locale_diagnostic_entry.h"
#include "hns/components/hns_ads/core/internal/diagnostics/entries/opted_in_to_hns_news_ads_diagnostic_entry.h"
#include "hns/components/hns_ads/core/internal/diagnostics/entries/opted_in_to_new_tab_page_ads_diagnostic_entry.h"
#include "hns/components/hns_ads/core/internal/diagnostics/entries/opted_in_to_notification_ads_diagnostic_entry.h"
#include "hns/components/hns_ads/core/internal/global_state/global_state.h"

namespace hns_ads {

DiagnosticManager::DiagnosticManager() {
  SetEntry(std::make_unique<CatalogIdDiagnosticEntry>());
  SetEntry(std::make_unique<CatalogLastUpdatedDiagnosticEntry>());
  SetEntry(std::make_unique<DeviceIdDiagnosticEntry>());
  SetEntry(std::make_unique<LastUnIdleTimeDiagnosticEntry>());
  SetEntry(std::make_unique<LocaleDiagnosticEntry>());
  SetEntry(std::make_unique<OptedInToHnsNewsAdsDiagnosticEntry>());
  SetEntry(std::make_unique<OptedInToNewTabPageAdsDiagnosticEntry>());
  SetEntry(std::make_unique<OptedInToNotificationAdsDiagnosticEntry>());
}

DiagnosticManager::~DiagnosticManager() = default;

// static
DiagnosticManager& DiagnosticManager::GetInstance() {
  return GlobalState::GetInstance()->GetDiagnosticManager();
}

void DiagnosticManager::SetEntry(
    std::unique_ptr<DiagnosticEntryInterface> entry) {
  CHECK(entry);

  const DiagnosticEntryType type = entry->GetType();
  diagnostics_[type] = std::move(entry);
}

void DiagnosticManager::GetDiagnostics(GetDiagnosticsCallback callback) const {
  std::move(callback).Run(DiagnosticsToValue(diagnostics_));
}

}  // namespace hns_ads
