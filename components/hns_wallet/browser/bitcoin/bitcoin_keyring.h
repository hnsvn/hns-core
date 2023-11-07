/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_BITCOIN_BITCOIN_KEYRING_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_BITCOIN_BITCOIN_KEYRING_H_

#include <memory>
#include <string>
#include <vector>

#include "hns/components/hns_wallet/browser/hd_keyring.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"

namespace hns_wallet {

class BitcoinKeyring : public HDKeyring {
 public:
  explicit BitcoinKeyring(bool testnet);
  ~BitcoinKeyring() override = default;
  BitcoinKeyring(const BitcoinKeyring&) = delete;
  BitcoinKeyring& operator=(const BitcoinKeyring&) = delete;

  absl::optional<std::string> GetAddress(const mojom::BitcoinKeyId& key_id);

  absl::optional<std::vector<uint8_t>> GetPubkey(
      const mojom::BitcoinKeyId& key_id);

  absl::optional<std::vector<uint8_t>> SignMessage(
      const mojom::BitcoinKeyId& key_id,
      base::span<const uint8_t, 32> message);

 private:
  std::string GetAddressInternal(HDKeyBase* hd_key_base) const override;
  std::unique_ptr<HDKeyBase> DeriveAccount(uint32_t index) const override;
  std::unique_ptr<HDKeyBase> DeriveKey(const mojom::BitcoinKeyId& key_id);

  bool testnet_ = false;
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_BITCOIN_BITCOIN_KEYRING_H_
