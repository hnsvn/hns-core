/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_COMMON_TEST_UTILS_H_
#define HNS_COMPONENTS_HNS_WALLET_COMMON_TEST_UTILS_H_

#include <string>

#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "testing/gmock/include/gmock/gmock-matchers.h"

namespace hns_wallet {

bool AllCoinsTested();

mojom::NetworkInfo GetTestNetworkInfo1(
    const std::string& chain_id = "chain_id",
    mojom::CoinType coin = mojom::CoinType::ETH);
mojom::NetworkInfo GetTestNetworkInfo2(
    const std::string& chain_id = "chain_id2",
    mojom::CoinType coin = mojom::CoinType::ETH);

// Matcher to check equality of two mojo structs. Matcher needs copyable value
// which is not possible for some mojo types, so wrapping it with RefCounted.
template <typename T>
auto EqualsMojo(const T& value) {
  return testing::Truly(
      [value = base::MakeRefCounted<base::RefCountedData<T>>(value.Clone())](
          const T& candidate) { return mojo::Equals(candidate, value->data); });
}

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_COMMON_TEST_UTILS_H_
