/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/catalog_permission_rule_unittest_util.h"

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"

namespace hns_ads {

void ForceCatalogPermissionRuleForTesting() {
  SetCatalogId(kCatalogId);
  SetCatalogVersion(1);
  SetCatalogPing(base::Hours(2));
  SetCatalogLastUpdated(Now());
}

}  // namespace hns_ads
