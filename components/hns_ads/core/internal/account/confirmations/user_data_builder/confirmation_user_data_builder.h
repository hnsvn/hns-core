/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_USER_DATA_BUILDER_CONFIRMATION_USER_DATA_BUILDER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_USER_DATA_BUILDER_CONFIRMATION_USER_DATA_BUILDER_H_

#include "base/functional/callback_forward.h"

namespace hns_ads {

struct TransactionInfo;
struct UserDataInfo;

using BuildConfirmationUserDataCallback =
    base::OnceCallback<void(const UserDataInfo& user_data)>;

void BuildConfirmationUserData(const TransactionInfo& transaction,
                               BuildConfirmationUserDataCallback callback);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_USER_DATA_BUILDER_CONFIRMATION_USER_DATA_BUILDER_H_
