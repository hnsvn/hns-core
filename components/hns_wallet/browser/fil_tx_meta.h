/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_FIL_TX_META_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_FIL_TX_META_H_

#include <memory>
#include <utility>

#include "hns/components/hns_wallet/browser/fil_transaction.h"
#include "hns/components/hns_wallet/browser/tx_meta.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"

namespace base {
class Value;
}  // namespace base

namespace hns_wallet {

class FilTransaction;

class FilTxMeta : public TxMeta {
 public:
  FilTxMeta();
  FilTxMeta(const mojom::AccountIdPtr& from,
            std::unique_ptr<FilTransaction> tx);
  FilTxMeta(const FilTxMeta&) = delete;
  FilTxMeta operator=(const FilTxMeta&) = delete;
  bool operator==(const FilTxMeta&) const;
  ~FilTxMeta() override;

  // TxMeta
  base::Value::Dict ToValue() const override;
  mojom::TransactionInfoPtr ToTransactionInfo() const override;

  FilTransaction* tx() const { return tx_.get(); }
  void set_tx(std::unique_ptr<FilTransaction> tx) { tx_ = std::move(tx); }

 private:
  std::unique_ptr<FilTransaction> tx_;
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_FIL_TX_META_H_
