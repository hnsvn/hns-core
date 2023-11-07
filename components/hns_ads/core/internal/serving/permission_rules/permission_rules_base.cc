/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rules_base.h"

#include "hns/components/hns_ads/core/internal/serving/permission_rules/command_line_permission_rule.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/confirmation_tokens_permission_rule.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/issuers_permission_rule.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rule_util.h"

namespace hns_ads {

PermissionRulesBase::PermissionRulesBase() = default;

PermissionRulesBase::~PermissionRulesBase() = default;

// static
bool PermissionRulesBase::HasPermission() {
  const IssuersPermissionRule issuers_permission_rule;
  if (!ShouldAllow(issuers_permission_rule)) {
    return false;
  }

  const ConfirmationTokensPermissionRule confirmation_tokens_permission_rule;
  if (!ShouldAllow(confirmation_tokens_permission_rule)) {
    return false;
  }

  const CommandLinePermissionRule catalog_permission_rule;
  return ShouldAllow(catalog_permission_rule);
}

}  // namespace hns_ads
