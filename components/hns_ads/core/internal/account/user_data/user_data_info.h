/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_USER_DATA_USER_DATA_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_USER_DATA_USER_DATA_INFO_H_

#include "base/values.h"

namespace hns_ads {

struct UserDataInfo final {
  UserDataInfo();

  UserDataInfo(const UserDataInfo&);
  UserDataInfo& operator=(const UserDataInfo&);

  UserDataInfo(UserDataInfo&&) noexcept;
  UserDataInfo& operator=(UserDataInfo&&) noexcept;

  ~UserDataInfo();

  base::Value::Dict dynamic;
  base::Value::Dict fixed;
};

bool operator==(const UserDataInfo&, const UserDataInfo&);
bool operator!=(const UserDataInfo&, const UserDataInfo&);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_USER_DATA_USER_DATA_INFO_H_
