/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/diagnostics/entries/opted_in_to_new_tab_page_ads_diagnostic_entry.h"

#include "hns/components/hns_ads/core/internal/common/strings/string_conversions_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"

namespace hns_ads {

namespace {
constexpr char kName[] = "Opted-in to new tab page ads";
}  // namespace

DiagnosticEntryType OptedInToNewTabPageAdsDiagnosticEntry::GetType() const {
  return DiagnosticEntryType::kOptedInToNewTabPageAds;
}

std::string OptedInToNewTabPageAdsDiagnosticEntry::GetName() const {
  return kName;
}

std::string OptedInToNewTabPageAdsDiagnosticEntry::GetValue() const {
  return BoolToString(UserHasOptedInToNewTabPageAds());
}

}  // namespace hns_ads
