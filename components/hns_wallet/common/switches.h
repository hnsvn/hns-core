// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_WALLET_COMMON_SWITCHES_H_
#define HNS_COMPONENTS_HNS_WALLET_COMMON_SWITCHES_H_

namespace hns_wallet {
namespace switches {

// Allows auto unlocking wallet password with command line.
extern const char kDevWalletPassword[];

// Bitcoin rpc mainnet endpoint.
extern const char kBitcoinMainnetRpcUrl[];

// Bitcoin rpc testnet endpoint.
extern const char kBitcoinTestnetRpcUrl[];

// Ratios service dev URL
extern const char kAssetRatioDevUrl[];

}  // namespace switches
}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_COMMON_SWITCHES_H_
