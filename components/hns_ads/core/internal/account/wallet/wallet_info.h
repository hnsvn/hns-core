/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_WALLET_WALLET_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_WALLET_WALLET_INFO_H_

#include <string>

namespace hns_ads {

struct WalletInfo final {
  [[nodiscard]] bool IsValid() const;

  bool operator==(const WalletInfo&) const;
  bool operator!=(const WalletInfo&) const;

  std::string payment_id;
  std::string public_key;
  std::string secret_key;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_WALLET_WALLET_INFO_H_
