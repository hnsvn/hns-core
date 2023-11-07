/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_USER_DATA_BUILDER_FIXED_CONFIRMATION_FIXED_USER_DATA_BUILDER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_USER_DATA_BUILDER_FIXED_CONFIRMATION_FIXED_USER_DATA_BUILDER_H_

#include "hns/components/hns_ads/core/internal/account/transactions/transaction_info.h"
#include "hns/components/hns_ads/core/internal/account/user_data/build_user_data_callback.h"

namespace hns_ads {

void BuildFixedUserData(const TransactionInfo& transaction,
                        BuildUserDataCallback callback);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_USER_DATA_BUILDER_FIXED_CONFIRMATION_FIXED_USER_DATA_BUILDER_H_
