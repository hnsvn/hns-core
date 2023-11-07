/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/diagnostics/diagnostic_value_util.h"

#include <utility>

#include "base/check.h"

namespace hns_ads {

namespace {

constexpr char kNameKey[] = "name";
constexpr char kValueKey[] = "value";

}  // namespace

base::Value::List DiagnosticsToValue(const DiagnosticMap& diagnostics) {
  base::Value::List list;

  for (const auto& [entry_type, entry] : diagnostics) {
    CHECK(entry);

    auto dict = base::Value::Dict()
                    .Set(kNameKey, entry->GetName())
                    .Set(kValueKey, entry->GetValue());

    list.Append(std::move(dict));
  }

  return list;
}

}  // namespace hns_ads
