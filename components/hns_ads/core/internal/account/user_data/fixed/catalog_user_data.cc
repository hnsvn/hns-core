/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/catalog_user_data.h"

#include <utility>

#include "hns/components/hns_ads/core/internal/catalog/catalog_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"

namespace hns_ads {

namespace {

constexpr char kCatalogKey[] = "catalog";
constexpr char kIdKey[] = "id";

}  // namespace

base::Value::Dict BuildCatalogUserData() {
  base::Value::Dict user_data;

  if (!UserHasJoinedHnsRewards()) {
    return user_data;
  }

  base::Value::List list;
  auto dict = base::Value::Dict().Set(kIdKey, GetCatalogId());
  list.Append(std::move(dict));

  user_data.Set(kCatalogKey, std::move(list));

  return user_data;
}

}  // namespace hns_ads
