/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_SEARCH_RESULT_ADS_SEARCH_RESULT_AD_PERMISSION_RULES_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_SEARCH_RESULT_ADS_SEARCH_RESULT_AD_PERMISSION_RULES_H_

#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rules_base.h"

namespace hns_ads {

class SearchResultAdPermissionRules final : public PermissionRulesBase {
 public:
  static bool HasPermission();
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_SEARCH_RESULT_ADS_SEARCH_RESULT_AD_PERMISSION_RULES_H_
