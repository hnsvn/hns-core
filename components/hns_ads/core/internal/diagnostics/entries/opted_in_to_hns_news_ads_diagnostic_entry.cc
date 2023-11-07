/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/diagnostics/entries/opted_in_to_hns_news_ads_diagnostic_entry.h"

#include "hns/components/hns_ads/core/internal/common/strings/string_conversions_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"

namespace hns_ads {

namespace {
constexpr char kName[] = "Opted-in to Hns News ads";
}  // namespace

DiagnosticEntryType OptedInToHnsNewsAdsDiagnosticEntry::GetType() const {
  return DiagnosticEntryType::kOptedInToHnsNewsAds;
}

std::string OptedInToHnsNewsAdsDiagnosticEntry::GetName() const {
  return kName;
}

std::string OptedInToHnsNewsAdsDiagnosticEntry::GetValue() const {
  return BoolToString(UserHasOptedInToHnsNewsAds());
}

}  // namespace hns_ads
