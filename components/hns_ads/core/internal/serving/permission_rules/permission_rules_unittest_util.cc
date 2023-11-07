/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rules_unittest_util.h"

#include "hns/components/hns_ads/core/internal/serving/permission_rules/catalog_permission_rule_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/confirmation_tokens_permission_rule_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/issuers_permission_rule_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/user_activity_permission_rule_unittest_util.h"

namespace hns_ads {

void ForcePermissionRulesForTesting() {
  ForceCatalogPermissionRuleForTesting();
  ForceConfirmationTokensPermissionRuleForTesting();
  ForceIssuersPermissionRuleForTesting();
  ForceUserActivityPermissionRuleForTesting();
}

}  // namespace hns_ads
