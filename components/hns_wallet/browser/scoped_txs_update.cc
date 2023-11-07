/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/browser/scoped_txs_update.h"

#include "hns/components/hns_wallet/browser/tx_storage_delegate.h"

namespace hns_wallet {

ScopedTxsUpdate::ScopedTxsUpdate(TxStorageDelegate* delegate)
    : delegate_(delegate) {}

ScopedTxsUpdate::~ScopedTxsUpdate() {
  delegate_->ScheduleWrite();
}

base::Value::Dict& ScopedTxsUpdate::Get() {
  return delegate_->GetTxs();
}

}  // namespace hns_wallet
