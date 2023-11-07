/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/refill_confirmation_tokens.h"

#include <utility>

#include "base/check.h"
#include "base/functional/bind.h"
#include "base/json/json_reader.h"
#include "base/strings/strcat.h"
#include "base/time/time.h"
#include "base/types/expected.h"
#include "hns/components/hns_ads/core/internal/account/issuers/issuers_util.h"
#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens_util.h"
#include "hns/components/hns_ads/core/internal/account/tokens/token_generator_interface.h"
#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/refill_confirmation_tokens_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/url_requests/get_signed_tokens/get_signed_tokens_url_request_builder.h"
#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/url_requests/get_signed_tokens/get_signed_tokens_url_request_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/url_requests/request_signed_tokens/request_signed_tokens_url_request_builder.h"
#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/url_requests/request_signed_tokens/request_signed_tokens_url_request_util.h"
#include "hns/components/hns_ads/core/internal/account/wallet/wallet_info.h"
#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/blinded_token_util.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/common/net/http/http_status_code.h"
#include "hns/components/hns_ads/core/internal/common/url/url_request_string_util.h"
#include "hns/components/hns_ads/core/internal/common/url/url_response_string_util.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"
#include "net/http/http_status_code.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

namespace {
constexpr base::TimeDelta kRetryAfter = base::Seconds(15);
}  // namespace

RefillConfirmationTokens::RefillConfirmationTokens(
    TokenGeneratorInterface* token_generator)
    : token_generator_(token_generator) {
  CHECK(token_generator_);
}

RefillConfirmationTokens::~RefillConfirmationTokens() {
  delegate_ = nullptr;
}

void RefillConfirmationTokens::MaybeRefill(const WalletInfo& wallet) {
  CHECK(wallet.IsValid());

  if (is_processing_ || retry_timer_.IsRunning()) {
    return;
  }

  if (!HasIssuers()) {
    BLOG(0, "Failed to refill confirmation tokens due to missing issuers");
    return FailedToRefill(/*should_retry*/ false);
  }

  if (!ShouldRefillConfirmationTokens()) {
    return BLOG(
        1, "No need to refill confirmation tokens as we already have "
               << ConfirmationTokenCount()
               << " confirmation tokens which is above the minimum threshold");
  }

  wallet_ = wallet;

  Refill();
}

///////////////////////////////////////////////////////////////////////////////

void RefillConfirmationTokens::Refill() {
  CHECK(!is_processing_);

  is_processing_ = true;

  if (delegate_) {
    delegate_->OnWillRefillConfirmationTokens();
  }

  GenerateTokens();

  RequestSignedTokens();
}

void RefillConfirmationTokens::GenerateTokens() {
  const int count = CalculateAmountOfConfirmationTokensToRefill();
  tokens_ = token_generator_->Generate(count);
  blinded_tokens_ = cbr::BlindTokens(*tokens_);
}

bool RefillConfirmationTokens::ShouldRequestSignedTokens() const {
  return !nonce_ || nonce_->empty();
}

void RefillConfirmationTokens::RequestSignedTokens() {
  CHECK(blinded_tokens_);

  BLOG(1, "RequestSignedTokens");
  BLOG(2, "POST /v{version}/confirmation/token/{paymentId}");

  RequestSignedTokensUrlRequestBuilder url_request_builder(wallet_,
                                                           *blinded_tokens_);
  mojom::UrlRequestInfoPtr url_request = url_request_builder.Build();
  BLOG(6, UrlRequestToString(url_request));
  BLOG(7, UrlRequestHeadersToString(url_request));

  AdsClientHelper::GetInstance()->UrlRequest(
      std::move(url_request),
      base::BindOnce(&RefillConfirmationTokens::RequestSignedTokensCallback,
                     weak_factory_.GetWeakPtr()));
}

void RefillConfirmationTokens::RequestSignedTokensCallback(
    const mojom::UrlResponseInfo& url_response) {
  BLOG(1, "OnRequestSignedTokens");

  BLOG(6, UrlResponseToString(url_response));
  BLOG(7, UrlResponseHeadersToString(url_response));

  const auto result = HandleRequestSignedTokensUrlResponse(url_response);
  if (!result.has_value()) {
    const auto& [failure, should_retry] = result.error();

    BLOG(0, failure);
    return FailedToRefill(should_retry);
  }

  GetSignedTokens();
}

base::expected<void, std::tuple<std::string, bool>>
RefillConfirmationTokens::HandleRequestSignedTokensUrlResponse(
    const mojom::UrlResponseInfo& url_response) {
  if (url_response.status_code == net::kHttpUpgradeRequired) {
    return base::unexpected(std::make_tuple(
        "Failed to request signed tokens as a browser upgrade is required",
        /*should_retry*/ false));
  }

  if (url_response.status_code != net::HTTP_CREATED) {
    return base::unexpected(std::make_tuple("Failed to request signed tokens",
                                            /*should_retry*/ true));
  }

  const absl::optional<base::Value::Dict> dict =
      base::JSONReader::ReadDict(url_response.body);
  if (!dict) {
    return base::unexpected(std::make_tuple(
        base::StrCat({"Failed to parse response: ", url_response.body}),
        /*should_retry*/ false));
  }

  nonce_ = ParseNonce(*dict);
  if (!nonce_) {
    return base::unexpected(
        std::make_tuple("Failed to parse nonce", /*should_retry*/ false));
  }

  return base::ok();
}

void RefillConfirmationTokens::GetSignedTokens() {
  CHECK(nonce_);

  BLOG(1, "GetSignedTokens");
  BLOG(2, "GET /v{version}/confirmation/token/{paymentId}?nonce={nonce}");

  GetSignedTokensUrlRequestBuilder url_request_builder(wallet_, *nonce_);
  mojom::UrlRequestInfoPtr url_request = url_request_builder.Build();
  BLOG(6, UrlRequestToString(url_request));
  BLOG(7, UrlRequestHeadersToString(url_request));

  AdsClientHelper::GetInstance()->UrlRequest(
      std::move(url_request),
      base::BindOnce(&RefillConfirmationTokens::GetSignedTokensCallback,
                     weak_factory_.GetWeakPtr()));
}

void RefillConfirmationTokens::GetSignedTokensCallback(
    const mojom::UrlResponseInfo& url_response) {
  BLOG(1, "OnGetSignedTokens");

  BLOG(6, UrlResponseToString(url_response));
  BLOG(7, UrlResponseHeadersToString(url_response));

  const auto result = HandleGetSignedTokensUrlResponse(url_response);
  if (!result.has_value()) {
    const auto& [failure, should_retry] = result.error();

    BLOG(0, failure);
    return FailedToRefill(should_retry);
  }

  SuccessfullyRefilled();
}

base::expected<void, std::tuple<std::string, bool>>
RefillConfirmationTokens::HandleGetSignedTokensUrlResponse(
    const mojom::UrlResponseInfo& url_response) {
  CHECK(tokens_);
  CHECK(blinded_tokens_);

  if (url_response.status_code == net::kHttpUpgradeRequired) {
    return base::unexpected(
        std::make_tuple("Failed to get signed tokens as a browser upgrade is "
                        "required", /*should_retry*/
                        false));
  }

  if (url_response.status_code != net::HTTP_OK &&
      url_response.status_code != net::HTTP_UNAUTHORIZED) {
    return base::unexpected(std::make_tuple("Failed to get signed tokens",
                                            /*should_retry*/ true));
  }

  const absl::optional<base::Value::Dict> dict =
      base::JSONReader::ReadDict(url_response.body);
  if (!dict) {
    return base::unexpected(std::make_tuple(
        base::StrCat({"Failed to parse response: ", url_response.body}),
        /*should_retry*/ false));
  }

  if (url_response.status_code == net::HTTP_UNAUTHORIZED) {
    ParseAndRequireCaptcha(*dict);

    return base::unexpected(std::make_tuple(
        "Captcha is required to refill confirmation tokens", /*should_retry*/
        false));
  }

  const auto result =
      ParseAndUnblindSignedTokens(*dict, *tokens_, *blinded_tokens_);
  if (!result.has_value()) {
    BLOG(0, result.error());
    return base::unexpected(
        std::make_tuple("Failed to parse and unblinded signed tokens",
                        /*should_retry*/ false));
  }
  const auto& [unblinded_tokens, public_key] = result.value();
  BuildAndAddConfirmationTokens(unblinded_tokens, public_key, wallet_);

  return base::ok();
}

void RefillConfirmationTokens::ParseAndRequireCaptcha(
    const base::Value::Dict& dict) const {
  const absl::optional<std::string> captcha_id = ParseCaptchaId(dict);
  if (!captcha_id) {
    return;
  }

  if (delegate_) {
    delegate_->OnCaptchaRequiredToRefillConfirmationTokens(*captcha_id);
  }
}

void RefillConfirmationTokens::SuccessfullyRefilled() {
  StopRetrying();

  Reset();

  is_processing_ = false;

  if (delegate_) {
    delegate_->OnDidRefillConfirmationTokens();
  }
}

void RefillConfirmationTokens::FailedToRefill(const bool should_retry) {
  if (delegate_) {
    delegate_->OnFailedToRefillConfirmationTokens();
  }

  if (should_retry) {
    return Retry();
  }

  Reset();
}

void RefillConfirmationTokens::Retry() {
  const base::Time retry_at = retry_timer_.StartWithPrivacy(
      FROM_HERE, kRetryAfter,
      base::BindOnce(&RefillConfirmationTokens::RetryCallback,
                     weak_factory_.GetWeakPtr()));

  if (delegate_) {
    delegate_->OnWillRetryRefillingConfirmationTokens(retry_at);
  }
}

void RefillConfirmationTokens::RetryCallback() {
  if (delegate_) {
    delegate_->OnDidRetryRefillingConfirmationTokens();
  }

  ShouldRequestSignedTokens() ? RequestSignedTokens() : GetSignedTokens();
}

void RefillConfirmationTokens::StopRetrying() {
  retry_timer_.Stop();
}

void RefillConfirmationTokens::Reset() {
  nonce_.reset();

  tokens_.reset();
  blinded_tokens_.reset();
}

}  // namespace hns_ads
