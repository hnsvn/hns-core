/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_PAYMENT_TOKENS_USER_DATA_REDEEM_PAYMENT_TOKENS_USER_DATA_BUILDER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_PAYMENT_TOKENS_USER_DATA_REDEEM_PAYMENT_TOKENS_USER_DATA_BUILDER_H_

#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_token_info.h"
#include "hns/components/hns_ads/core/internal/account/user_data/build_user_data_callback.h"

namespace hns_ads {

void BuildRedeemPaymentTokensUserData(const PaymentTokenList& payment_tokens,
                                      BuildUserDataCallback callback);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_PAYMENT_TOKENS_USER_DATA_REDEEM_PAYMENT_TOKENS_USER_DATA_BUILDER_H_
