/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REWARD_URL_REQUEST_BUILDERS_CREATE_REWARD_CONFIRMATION_URL_REQUEST_BUILDER_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REWARD_URL_REQUEST_BUILDERS_CREATE_REWARD_CONFIRMATION_URL_REQUEST_BUILDER_UTIL_H_

#include <string>

namespace hns_ads {

std::string BuildCreateRewardConfirmationUrlPath(
    const std::string& transaction_id,
    const std::string& credential_base64_url);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REWARD_URL_REQUEST_BUILDERS_CREATE_REWARD_CONFIRMATION_URL_REQUEST_BUILDER_UTIL_H_
