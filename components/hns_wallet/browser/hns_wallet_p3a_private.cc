/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/browser/hns_wallet_p3a_private.h"

namespace hns_wallet {

void HnsWalletP3APrivate::ReportJSProvider(
    mojom::JSProviderType provider_type,
    mojom::CoinType coin_type,
    bool allow_provider_overwrite) {}

void HnsWalletP3APrivate::ReportOnboardingAction(
    mojom::OnboardingAction onboarding_action) {}

void HnsWalletP3APrivate::ReportTransactionSent(mojom::CoinType coin,
                                                  bool new_send) {}

void HnsWalletP3APrivate::RecordActiveWalletCount(int count,
                                                    mojom::CoinType coin_type) {
}

void HnsWalletP3APrivate::RecordNFTGalleryView(int nft_count) {}

}  // namespace hns_wallet
