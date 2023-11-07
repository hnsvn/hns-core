/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rule_util.h"

#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rule_interface.h"

namespace hns_ads {

bool ShouldAllow(const PermissionRuleInterface& permission_rule) {
  const auto result = permission_rule.ShouldAllow();
  if (!result.has_value()) {
    BLOG(2, result.error());
    return false;
  }

  return true;
}

}  // namespace hns_ads
