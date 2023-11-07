/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_PAYMENT_TOKENS_URL_REQUEST_BUILDERS_REDEEM_PAYMENT_TOKENS_URL_REQUEST_BUILDER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_PAYMENT_TOKENS_URL_REQUEST_BUILDERS_REDEEM_PAYMENT_TOKENS_URL_REQUEST_BUILDER_H_

#include <string>

#include "base/values.h"
#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_token_info.h"
#include "hns/components/hns_ads/core/internal/account/wallet/wallet_info.h"
#include "hns/components/hns_ads/core/internal/common/url/request_builder/url_request_builder_interface.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

class GURL;

namespace hns_ads {

class RedeemPaymentTokensUrlRequestBuilder final
    : public UrlRequestBuilderInterface {
 public:
  RedeemPaymentTokensUrlRequestBuilder(WalletInfo wallet,
                                       PaymentTokenList payment_tokens,
                                       base::Value::Dict user_data);

  RedeemPaymentTokensUrlRequestBuilder(
      const RedeemPaymentTokensUrlRequestBuilder&) = delete;
  RedeemPaymentTokensUrlRequestBuilder& operator=(
      const RedeemPaymentTokensUrlRequestBuilder&) = delete;

  RedeemPaymentTokensUrlRequestBuilder(
      RedeemPaymentTokensUrlRequestBuilder&&) noexcept = delete;
  RedeemPaymentTokensUrlRequestBuilder& operator=(
      RedeemPaymentTokensUrlRequestBuilder&&) noexcept = delete;

  ~RedeemPaymentTokensUrlRequestBuilder() override;

  mojom::UrlRequestInfoPtr Build() override;

 private:
  GURL BuildUrl() const;

  std::string BuildBody(const std::string& payload);

  std::string BuildPayload() const;

  base::Value::List BuildPaymentRequestDTO(const std::string& payload) const;

  WalletInfo wallet_;
  PaymentTokenList payment_tokens_;
  base::Value::Dict user_data_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_PAYMENT_TOKENS_URL_REQUEST_BUILDERS_REDEEM_PAYMENT_TOKENS_URL_REQUEST_BUILDER_H_
