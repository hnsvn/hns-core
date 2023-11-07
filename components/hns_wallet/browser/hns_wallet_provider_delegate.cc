/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/browser/hns_wallet_provider_delegate.h"

namespace hns_wallet {

bool HnsWalletProviderDelegate::IsSolanaAccountConnected(
    const std::string& account) {
  return false;
}

}  // namespace hns_wallet
