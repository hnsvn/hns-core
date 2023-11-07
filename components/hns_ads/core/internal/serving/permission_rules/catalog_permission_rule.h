/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_CATALOG_PERMISSION_RULE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_CATALOG_PERMISSION_RULE_H_

#include <string>

#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rule_interface.h"

namespace hns_ads {

class CatalogPermissionRule final : public PermissionRuleInterface {
 public:
  base::expected<void, std::string> ShouldAllow() const override;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_CATALOG_PERMISSION_RULE_H_
