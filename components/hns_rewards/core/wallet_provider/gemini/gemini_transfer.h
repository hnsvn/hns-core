/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_GEMINI_GEMINI_TRANSFER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_GEMINI_GEMINI_TRANSFER_H_

#include "hns/components/hns_rewards/core/endpoints/gemini/post_commit_transaction_gemini.h"
#include "hns/components/hns_rewards/core/wallet_provider/transfer.h"

namespace hns_rewards::internal::gemini {

class GeminiTransfer final : public wallet_provider::Transfer {
 public:
  using Transfer::Transfer;

 private:
  void CommitTransaction(ResultCallback,
                         mojom::ExternalTransactionPtr) const override;

  void OnCommitTransaction(
      ResultCallback,
      endpoints::PostCommitTransactionGemini::Result&&) const;
};

}  // namespace hns_rewards::internal::gemini

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_WALLET_PROVIDER_GEMINI_GEMINI_TRANSFER_H_
