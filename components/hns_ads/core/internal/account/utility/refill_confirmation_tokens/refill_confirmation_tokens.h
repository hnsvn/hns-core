/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REFILL_CONFIRMATION_TOKENS_REFILL_CONFIRMATION_TOKENS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REFILL_CONFIRMATION_TOKENS_REFILL_CONFIRMATION_TOKENS_H_

#include <string>
#include <tuple>
#include <vector>

#include "base/check_op.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/refill_confirmation_tokens_delegate.h"
#include "hns/components/hns_ads/core/internal/account/wallet/wallet_info.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/blinded_token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token.h"
#include "hns/components/hns_ads/core/internal/common/timer/backoff_timer.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

namespace hns_ads {

class TokenGeneratorInterface;

class RefillConfirmationTokens final {
 public:
  explicit RefillConfirmationTokens(TokenGeneratorInterface* token_generator);

  RefillConfirmationTokens(const RefillConfirmationTokens&) = delete;
  RefillConfirmationTokens& operator=(const RefillConfirmationTokens&) = delete;

  RefillConfirmationTokens(RefillConfirmationTokens&&) noexcept = delete;
  RefillConfirmationTokens& operator=(RefillConfirmationTokens&&) noexcept =
      delete;

  ~RefillConfirmationTokens();

  void SetDelegate(RefillConfirmationTokensDelegate* delegate) {
    CHECK_EQ(delegate_, nullptr);
    delegate_ = delegate;
  }

  void MaybeRefill(const WalletInfo& wallet);

 private:
  void Refill();

  void GenerateTokens();

  bool ShouldRequestSignedTokens() const;
  void RequestSignedTokens();
  void RequestSignedTokensCallback(const mojom::UrlResponseInfo& url_response);
  base::expected<void, std::tuple<std::string, /*should_retry*/ bool>>
  HandleRequestSignedTokensUrlResponse(
      const mojom::UrlResponseInfo& url_response);

  void GetSignedTokens();
  void GetSignedTokensCallback(const mojom::UrlResponseInfo& url_response);
  base::expected<void, std::tuple<std::string, /*should_retry*/ bool>>
  HandleGetSignedTokensUrlResponse(const mojom::UrlResponseInfo& url_response);
  void ParseAndRequireCaptcha(const base::Value::Dict& dict) const;

  void SuccessfullyRefilled();
  void FailedToRefill(bool should_retry);

  void Retry();
  void RetryCallback();
  void StopRetrying();

  void Reset();

  const raw_ptr<TokenGeneratorInterface> token_generator_ =
      nullptr;  // NOT OWNED

  raw_ptr<RefillConfirmationTokensDelegate> delegate_ = nullptr;

  WalletInfo wallet_;

  absl::optional<std::string> nonce_;

  absl::optional<std::vector<cbr::Token>> tokens_;
  absl::optional<std::vector<cbr::BlindedToken>> blinded_tokens_;

  bool is_processing_ = false;

  BackoffTimer retry_timer_;

  base::WeakPtrFactory<RefillConfirmationTokens> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REFILL_CONFIRMATION_TOKENS_REFILL_CONFIRMATION_TOKENS_H_
