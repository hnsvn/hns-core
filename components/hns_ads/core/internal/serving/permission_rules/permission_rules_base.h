/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_PERMISSION_RULES_BASE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_PERMISSION_RULES_BASE_H_

namespace hns_ads {

class PermissionRulesBase {
 public:
  PermissionRulesBase(const PermissionRulesBase&) = delete;
  PermissionRulesBase& operator=(const PermissionRulesBase&) = delete;

  PermissionRulesBase(PermissionRulesBase&&) noexcept = delete;
  PermissionRulesBase& operator=(PermissionRulesBase&&) noexcept = delete;

  virtual ~PermissionRulesBase();

 protected:
  PermissionRulesBase();

  static bool HasPermission();
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_PERMISSION_RULES_PERMISSION_RULES_BASE_H_
