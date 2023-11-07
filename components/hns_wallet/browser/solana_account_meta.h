/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_SOLANA_ACCOUNT_META_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_SOLANA_ACCOUNT_META_H_

#include <string>
#include <vector>

#include "base/values.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_wallet {

struct SolanaAccountMeta {
  SolanaAccountMeta(const std::string& pubkey,
                    absl::optional<uint8_t> address_table_lookup_index,
                    bool is_signer,
                    bool is_writable);
  ~SolanaAccountMeta();

  SolanaAccountMeta(const SolanaAccountMeta&);
  bool operator==(const SolanaAccountMeta&) const;

  mojom::SolanaAccountMetaPtr ToMojomSolanaAccountMeta() const;
  base::Value::Dict ToValue() const;

  static void FromMojomSolanaAccountMetas(
      const std::vector<mojom::SolanaAccountMetaPtr>& mojom_account_metas,
      std::vector<SolanaAccountMeta>* account_metas);
  static absl::optional<SolanaAccountMeta> FromValue(
      const base::Value::Dict& value);

  std::string pubkey;
  absl::optional<uint8_t> address_table_lookup_index;
  bool is_signer;
  bool is_writable;
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_SOLANA_ACCOUNT_META_H_
