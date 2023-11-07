/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/wallet/wallet_unittest_util.h"

#include "hns/components/hns_ads/core/internal/account/wallet/wallet_info.h"
#include "hns/components/hns_ads/core/internal/account/wallet/wallet_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/account/wallet/wallet_util.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"

namespace hns_ads {

WalletInfo GetWalletForTesting() {
  return ToWallet(kWalletPaymentId, kWalletRecoverySeed).value_or(WalletInfo{});
}

mojom::WalletInfoPtr GetWalletPtrForTesting() {
  mojom::WalletInfoPtr wallet = mojom::WalletInfo::New();
  wallet->payment_id = kWalletPaymentId;
  wallet->recovery_seed = kWalletRecoverySeed;
  return wallet;
}

}  // namespace hns_ads
