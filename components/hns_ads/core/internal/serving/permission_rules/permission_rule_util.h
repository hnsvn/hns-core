/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_PERMISSION_RULE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_PERMISSION_RULE_UTIL_H_

namespace hns_ads {

class PermissionRuleInterface;

bool ShouldAllow(const PermissionRuleInterface& permission_rule);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_PERMISSION_RULE_UTIL_H_
