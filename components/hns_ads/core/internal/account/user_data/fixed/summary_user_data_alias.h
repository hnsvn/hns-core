/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_USER_DATA_FIXED_SUMMARY_USER_DATA_ALIAS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_USER_DATA_FIXED_SUMMARY_USER_DATA_ALIAS_H_

#include <string>

#include "base/containers/flat_map.h"

namespace hns_ads {

using ConfirmationTypeBucketMap =
    base::flat_map</*confirmation_type*/ std::string, /*count*/ int>;
using AdTypeBucketMap =
    base::flat_map</*ad_type*/ std::string, ConfirmationTypeBucketMap>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_USER_DATA_FIXED_SUMMARY_USER_DATA_ALIAS_H_
