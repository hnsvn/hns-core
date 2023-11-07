/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_REWARD_REWARD_CONFIRMATION_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_REWARD_REWARD_CONFIRMATION_UTIL_H_

#include <string>

#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

struct ConfirmationInfo;
struct UserDataInfo;
class TokenGeneratorInterface;
struct TransactionInfo;

absl::optional<std::string> BuildRewardCredential(
    const ConfirmationInfo& confirmation);

absl::optional<ConfirmationInfo> BuildRewardConfirmation(
    TokenGeneratorInterface* token_generator,
    const TransactionInfo& transaction,
    const UserDataInfo& user_data);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_REWARD_REWARD_CONFIRMATION_UTIL_H_
