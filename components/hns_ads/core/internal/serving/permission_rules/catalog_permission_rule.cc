/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/catalog_permission_rule.h"

#include "hns/components/hns_ads/core/internal/catalog/catalog_util.h"

namespace hns_ads {

base::expected<void, std::string> CatalogPermissionRule::ShouldAllow() const {
  if (!DoesCatalogExist()) {
    return base::unexpected("Catalog does not exist");
  }

  if (HasCatalogExpired()) {
    return base::unexpected("Catalog has expired");
  }

  return base::ok();
}

}  // namespace hns_ads
