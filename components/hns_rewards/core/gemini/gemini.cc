/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/gemini/gemini.h"

#include <memory>
#include <utility>

#include "hns/components/hns_rewards/common/mojom/rewards_types.mojom.h"
#include "hns/components/hns_rewards/core/buildflags.h"
#include "hns/components/hns_rewards/core/gemini/gemini_util.h"
#include "hns/components/hns_rewards/core/global_constants.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"
#include "hns/components/hns_rewards/core/wallet_provider/gemini/connect_gemini_wallet.h"
#include "hns/components/hns_rewards/core/wallet_provider/gemini/gemini_transfer.h"
#include "hns/components/hns_rewards/core/wallet_provider/gemini/get_gemini_wallet.h"

namespace hns_rewards::internal::gemini {

Gemini::Gemini(RewardsEngineImpl& engine)
    : WalletProvider(engine), server_(engine) {
  connect_wallet_ = std::make_unique<ConnectGeminiWallet>(engine);
  get_wallet_ = std::make_unique<GetGeminiWallet>(engine);
  transfer_ = std::make_unique<GeminiTransfer>(engine);
}

const char* Gemini::WalletType() const {
  return constant::kWalletGemini;
}

void Gemini::FetchBalance(
    base::OnceCallback<void(mojom::Result, double)> callback) {
  auto wallet = GetWalletIf({mojom::WalletStatus::kConnected});
  if (!wallet) {
    return std::move(callback).Run(mojom::Result::FAILED, 0.0);
  }

  auto url_callback = base::BindOnce(
      &Gemini::OnFetchBalance, base::Unretained(this), std::move(callback));

  server_.post_balance().Request(wallet->token, std::move(url_callback));
}

std::string Gemini::GetFeeAddress() const {
  return gemini::GetFeeAddress();
}

base::TimeDelta Gemini::GetDelay() const {
  return base::Minutes(5);
}

}  // namespace hns_rewards::internal::gemini
