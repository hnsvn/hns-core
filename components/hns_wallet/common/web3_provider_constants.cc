/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/common/web3_provider_constants.h"

namespace hns_wallet {

const char kConnectEvent[] = "connect";
const char kDisconnectEvent[] = "disconnect";

namespace ethereum {
const char kChainChangedEvent[] = "chainChanged";
const char kAccountsChangedEvent[] = "accountsChanged";
const char kMessageEvent[] = "message";
}  // namespace ethereum

namespace solana {
const char kAccountChangedEvent[] = "accountChanged";
}  // namespace solana

}  // namespace hns_wallet