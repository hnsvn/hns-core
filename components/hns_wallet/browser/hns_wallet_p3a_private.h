/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_P3A_PRIVATE_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_P3A_PRIVATE_H_

#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"

namespace hns_wallet {

class HnsWalletP3APrivate : public mojom::HnsWalletP3A {
 public:
  HnsWalletP3APrivate() = default;

  void ReportJSProvider(mojom::JSProviderType provider_type,
                        mojom::CoinType coin_type,
                        bool allow_provider_overwrite) override;
  void ReportOnboardingAction(
      mojom::OnboardingAction onboarding_action) override;
  void ReportTransactionSent(mojom::CoinType coin, bool new_send) override;
  void RecordActiveWalletCount(int count, mojom::CoinType coin_type) override;
  void RecordNFTGalleryView(int nft_count) override;
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_P3A_PRIVATE_H_
