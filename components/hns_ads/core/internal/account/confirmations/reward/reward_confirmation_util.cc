/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/confirmations/reward/reward_confirmation_util.h"

#include <vector>

#include "base/base64url.h"
#include "base/check.h"
#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/confirmation_info.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/confirmations_util.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/payload/confirmation_payload_json_writer.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/reward/reward_credential_json_writer.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/reward/reward_info.h"
#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_token_info.h"
#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens_util.h"
#include "hns/components/hns_ads/core/internal/account/tokens/token_generator_interface.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transaction_info.h"
#include "hns/components/hns_ads/core/internal/account/user_data/user_data_info.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/blinded_token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/blinded_token_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

namespace hns_ads {

namespace {

absl::optional<RewardInfo> BuildReward(TokenGeneratorInterface* token_generator,
                                       const ConfirmationInfo& confirmation) {
  CHECK(token_generator);
  CHECK(IsValid(confirmation));
  CHECK(UserHasJoinedHnsRewards());

  RewardInfo reward;

  // Token
  const std::vector<cbr::Token> tokens = token_generator->Generate(/*count*/ 1);
  CHECK(!tokens.empty());
  reward.token = tokens.front();

  // Blinded token
  const std::vector<cbr::BlindedToken> blinded_tokens =
      cbr::BlindTokens(tokens);
  CHECK(!blinded_tokens.empty());
  reward.blinded_token = blinded_tokens.front();

  // Confirmation token
  const absl::optional<ConfirmationTokenInfo> confirmation_token =
      MaybeGetConfirmationToken();
  if (!confirmation_token) {
    BLOG(0, "Failed to get confirmation token");
    return absl::nullopt;
  }

  if (!RemoveConfirmationToken(*confirmation_token)) {
    BLOG(0, "Failed to remove confirmation token");
    return absl::nullopt;
  }

  reward.unblinded_token = confirmation_token->unblinded_token;
  reward.public_key = confirmation_token->public_key;
  reward.signature = confirmation_token->signature;

  // Credential
  ConfirmationInfo mutable_confirmation(confirmation);
  mutable_confirmation.reward = reward;

  const absl::optional<std::string> reward_credential_base64url =
      BuildRewardCredential(mutable_confirmation);
  if (!reward_credential_base64url) {
    return absl::nullopt;
  }
  reward.credential_base64url = *reward_credential_base64url;

  return reward;
}

}  // namespace

absl::optional<std::string> BuildRewardCredential(
    const ConfirmationInfo& confirmation) {
  const absl::optional<std::string> reward_credential =
      json::writer::WriteRewardCredential(
          confirmation.reward,
          json::writer::WriteConfirmationPayload(confirmation));
  if (!reward_credential) {
    BLOG(0, "Failed to create reward credential");
    return absl::nullopt;
  }

  std::string reward_credential_base64url;
  base::Base64UrlEncode(*reward_credential,
                        base::Base64UrlEncodePolicy::INCLUDE_PADDING,
                        &reward_credential_base64url);
  return reward_credential_base64url;
}

absl::optional<ConfirmationInfo> BuildRewardConfirmation(
    TokenGeneratorInterface* token_generator,
    const TransactionInfo& transaction,
    const UserDataInfo& user_data) {
  CHECK(token_generator);
  CHECK(transaction.IsValid());
  CHECK(UserHasJoinedHnsRewards());

  ConfirmationInfo confirmation;
  confirmation.transaction_id = transaction.id;
  confirmation.creative_instance_id = transaction.creative_instance_id;
  confirmation.type = transaction.confirmation_type;
  confirmation.ad_type = transaction.ad_type;
  confirmation.created_at = transaction.created_at;
  confirmation.user_data = user_data;

  const absl::optional<RewardInfo> reward =
      BuildReward(token_generator, confirmation);
  if (!reward) {
    BLOG(0, "Failed to create reward");
    return absl::nullopt;
  }
  confirmation.reward = reward;

  return confirmation;
}

}  // namespace hns_ads
