/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_PAYMENT_TOKENS_PAYMENT_TOKEN_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_PAYMENT_TOKENS_PAYMENT_TOKEN_INFO_H_

#include <string>
#include <vector>

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/public_key.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/unblinded_token.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

namespace hns_ads {

struct PaymentTokenInfo final {
  PaymentTokenInfo();

  PaymentTokenInfo(const PaymentTokenInfo&);
  PaymentTokenInfo& operator=(const PaymentTokenInfo&);

  PaymentTokenInfo(PaymentTokenInfo&&) noexcept;
  PaymentTokenInfo& operator=(PaymentTokenInfo&&) noexcept;

  ~PaymentTokenInfo();

  bool operator==(const PaymentTokenInfo&) const;
  bool operator!=(const PaymentTokenInfo&) const;

  std::string transaction_id;
  cbr::UnblindedToken unblinded_token;
  cbr::PublicKey public_key;
  ConfirmationType confirmation_type = ConfirmationType::kUndefined;
  AdType ad_type = AdType::kUndefined;
};

using PaymentTokenList = std::vector<PaymentTokenInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TOKENS_PAYMENT_TOKENS_PAYMENT_TOKEN_INFO_H_
