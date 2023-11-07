/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/redeem_payment_tokens/user_data/redeem_payment_tokens_user_data_builder.h"

#include <utility>

#include "base/functional/callback.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/internal/account/user_data/build_user_data_callback.h"
#include "hns/components/hns_ads/core/internal/account/user_data/fixed/platform_user_data.h"
#include "hns/components/hns_ads/core/internal/account/user_data/fixed/summary_user_data.h"

namespace hns_ads {

void BuildRedeemPaymentTokensUserData(const PaymentTokenList& payment_tokens,
                                      BuildUserDataCallback callback) {
  base::Value::Dict user_data;

  user_data.Merge(BuildPlatformUserData());
  user_data.Merge(BuildSummaryUserData(payment_tokens));

  std::move(callback).Run(std::move(user_data));
}

}  // namespace hns_ads
