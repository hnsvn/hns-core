/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_DIAGNOSTIC_ALIAS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_DIAGNOSTIC_ALIAS_H_

#include <memory>

#include "base/containers/flat_map.h"
#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_entry_types.h"
#include "hns/components/hns_ads/core/internal/diagnostics/entries/diagnostic_entry_interface.h"

namespace hns_ads {

using DiagnosticMap = base::flat_map<DiagnosticEntryType,
                                     std::unique_ptr<DiagnosticEntryInterface>>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DIAGNOSTICS_DIAGNOSTIC_ALIAS_H_
