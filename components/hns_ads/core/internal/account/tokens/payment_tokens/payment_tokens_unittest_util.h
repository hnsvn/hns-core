/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_PAYMENT_TOKENS_PAYMENT_TOKENS_UNITTEST_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_PAYMENT_TOKENS_PAYMENT_TOKENS_UNITTEST_UTIL_H_

#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_token_info.h"

namespace hns_ads {

class AdType;
class PaymentTokens;

PaymentTokens& GetPaymentTokensForTesting();
PaymentTokenList SetPaymentTokensForTesting(int count);

PaymentTokenInfo BuildPaymentTokenForTesting(
    const ConfirmationType& confirmation_type,
    const AdType& ad_type);
PaymentTokenInfo BuildPaymentTokenForTesting();
PaymentTokenList BuildPaymentTokensForTesting(int count);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_PAYMENT_TOKENS_PAYMENT_TOKENS_UNITTEST_UTIL_H_
