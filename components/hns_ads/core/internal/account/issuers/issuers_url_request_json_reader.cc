/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/issuers/issuers_url_request_json_reader.h"

#include "base/json/json_reader.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/internal/account/issuers/issuer_info.h"
#include "hns/components/hns_ads/core/internal/account/issuers/issuers_info.h"
#include "hns/components/hns_ads/core/internal/account/issuers/issuers_url_request_json_reader_util.h"

namespace hns_ads::json::reader {

absl::optional<IssuersInfo> ReadIssuers(const std::string& json) {
  const absl::optional<base::Value> root = base::JSONReader::Read(json);
  if (!root || !root->is_dict()) {
    return absl::nullopt;
  }
  const base::Value::Dict& dict = root->GetDict();

  const absl::optional<int> ping = ParsePing(dict);
  if (!ping) {
    return absl::nullopt;
  }

  const absl::optional<IssuerList> issuers = ParseIssuers(dict);
  if (!issuers) {
    return absl::nullopt;
  }

  IssuersInfo new_issuers;
  new_issuers.ping = *ping;
  new_issuers.issuers = *issuers;

  return new_issuers;
}

}  // namespace hns_ads::json::reader
