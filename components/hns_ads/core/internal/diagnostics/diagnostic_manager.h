/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_DIAGNOSTIC_MANAGER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_DIAGNOSTIC_MANAGER_H_

#include <memory>

#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_alias.h"
#include "hns/components/hns_ads/core/public/ads_callback.h"

namespace hns_ads {

class DiagnosticManager final {
 public:
  DiagnosticManager();

  DiagnosticManager(const DiagnosticManager&) = delete;
  DiagnosticManager& operator=(const DiagnosticManager&) = delete;

  DiagnosticManager(DiagnosticManager&&) noexcept = delete;
  DiagnosticManager& operator=(DiagnosticManager&&) noexcept = delete;

  ~DiagnosticManager();

  static DiagnosticManager& GetInstance();

  void SetEntry(std::unique_ptr<DiagnosticEntryInterface> entry);

  void GetDiagnostics(GetDiagnosticsCallback callback) const;

 private:
  DiagnosticMap diagnostics_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_DIAGNOSTIC_MANAGER_H_
