/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/database/database_transaction_util.h"

#include <utility>

#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"

namespace hns_ads::database {

namespace {

void RunTransactionCallback(ResultCallback callback,
                            mojom::DBCommandResponseInfoPtr command_response) {
  if (!command_response) {
    return std::move(callback).Run(/*success*/ false);
  }

  std::move(callback).Run(
      /*success*/ command_response->status ==
      mojom::DBCommandResponseInfo::StatusType::RESPONSE_OK);
}

}  // namespace

void RunTransaction(mojom::DBTransactionInfoPtr transaction,
                    ResultCallback callback) {
  AdsClientHelper::GetInstance()->RunDBTransaction(
      std::move(transaction),
      base::BindOnce(&RunTransactionCallback, std::move(callback)));
}

}  // namespace hns_ads::database
