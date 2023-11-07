/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/redeem_payment_tokens/url_request_builders/redeem_payment_tokens_url_request_builder.h"

#include <utility>
#include <vector>

#include "base/check.h"
#include "base/json/json_writer.h"
#include "base/notreached.h"
#include "base/strings/string_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token_preimage.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_key.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_signature.h"
#include "hns/components/hns_ads/core/internal/common/url/request_builder/host/url_host_util.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/gurl.h"

namespace hns_ads {

namespace {

std::vector<std::string> BuildHeaders() {
  return {"accept: application/json"};
}

base::Value::Dict BuildCredential(const PaymentTokenInfo& payment_token,
                                  const std::string& payload) {
  CHECK(!payload.empty());

  absl::optional<cbr::VerificationKey> verification_key =
      payment_token.unblinded_token.DeriveVerificationKey();
  CHECK(verification_key);

  const absl::optional<cbr::VerificationSignature> verification_signature =
      verification_key->Sign(payload);
  CHECK(verification_signature);

  const absl::optional<std::string> verification_signature_base64 =
      verification_signature->EncodeBase64();
  CHECK(verification_signature_base64);

  const absl::optional<cbr::TokenPreimage> token_preimage =
      payment_token.unblinded_token.GetTokenPreimage();
  CHECK(token_preimage);

  const absl::optional<std::string> token_preimage_base64 =
      token_preimage->EncodeBase64();
  CHECK(token_preimage_base64);

  return base::Value::Dict()
      .Set("signature", *verification_signature_base64)
      .Set("t", *token_preimage_base64);
}

}  // namespace

RedeemPaymentTokensUrlRequestBuilder::RedeemPaymentTokensUrlRequestBuilder(
    WalletInfo wallet,
    PaymentTokenList payment_tokens,
    base::Value::Dict user_data)
    : wallet_(std::move(wallet)),
      payment_tokens_(std::move(payment_tokens)),
      user_data_(std::move(user_data)) {
  CHECK(wallet_.IsValid());
  CHECK(!payment_tokens_.empty());
}

RedeemPaymentTokensUrlRequestBuilder::~RedeemPaymentTokensUrlRequestBuilder() =
    default;

mojom::UrlRequestInfoPtr RedeemPaymentTokensUrlRequestBuilder::Build() {
  mojom::UrlRequestInfoPtr url_request = mojom::UrlRequestInfo::New();
  url_request->url = BuildUrl();
  url_request->headers = BuildHeaders();
  const std::string payload = BuildPayload();
  url_request->content = BuildBody(payload);
  url_request->content_type = "application/json";
  url_request->method = mojom::UrlRequestMethodType::kPut;

  return url_request;
}

///////////////////////////////////////////////////////////////////////////////

GURL RedeemPaymentTokensUrlRequestBuilder::BuildUrl() const {
  const std::string spec = base::ReplaceStringPlaceholders(
      "$1/v3/confirmation/payment/$2",
      {GetNonAnonymousUrlHost(), wallet_.payment_id}, nullptr);
  return GURL(spec);
}

std::string RedeemPaymentTokensUrlRequestBuilder::BuildBody(
    const std::string& payload) {
  CHECK(!payload.empty());
  CHECK(!user_data_.empty());

  auto dict = base::Value::Dict()
                  .Set("paymentCredentials", BuildPaymentRequestDTO(payload))
                  .Set("payload", payload);

  dict.Merge(std::move(user_data_));

  std::string json;
  CHECK(base::JSONWriter::Write(dict, &json));
  return json;
}

std::string RedeemPaymentTokensUrlRequestBuilder::BuildPayload() const {
  const auto dict = base::Value::Dict().Set("paymentId", wallet_.payment_id);

  std::string json;
  CHECK(base::JSONWriter::Write(dict, &json));
  return json;
}

base::Value::List RedeemPaymentTokensUrlRequestBuilder::BuildPaymentRequestDTO(
    const std::string& payload) const {
  CHECK(!payload.empty());

  base::Value::List list;

  for (const auto& payment_token : payment_tokens_) {
    auto dict = base::Value::Dict()
                    .Set("credential",
                         base::Value(BuildCredential(payment_token, payload)))
                    .Set("confirmationType",
                         payment_token.confirmation_type.ToString());

    const absl::optional<std::string> public_key_base64 =
        payment_token.public_key.EncodeBase64();
    if (!public_key_base64) {
      NOTREACHED_NORETURN();
    } else {
      dict.Set("publicKey", *public_key_base64);
    }

    list.Append(std::move(dict));
  }

  return list;
}

}  // namespace hns_ads
