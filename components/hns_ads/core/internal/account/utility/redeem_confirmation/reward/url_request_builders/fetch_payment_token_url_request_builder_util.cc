/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/reward/url_request_builders/fetch_payment_token_url_request_builder_util.h"

#include "base/strings/stringprintf.h"
#include "hns/components/hns_ads/core/internal/account/utility/tokens_constants.h"

namespace hns_ads {

std::string BuildFetchPaymentTokenUrlPath(const std::string& transaction_id) {
  return base::StringPrintf("/v%d/confirmation/%s/paymentToken",
                            kTokensServerVersion, transaction_id.c_str());
}

}  // namespace hns_ads
