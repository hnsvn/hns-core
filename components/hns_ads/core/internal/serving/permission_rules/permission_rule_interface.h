/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_PERMISSION_RULE_INTERFACE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_PERMISSION_RULE_INTERFACE_H_

#include <string>

#include "base/types/expected.h"

namespace hns_ads {

class PermissionRuleInterface {
 public:
  virtual ~PermissionRuleInterface() = default;

  virtual base::expected<void, std::string> ShouldAllow() const = 0;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_PERMISSION_RULE_INTERFACE_H_
