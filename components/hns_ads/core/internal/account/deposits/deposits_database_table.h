/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_DEPOSITS_DEPOSITS_DATABASE_TABLE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_DEPOSITS_DEPOSITS_DATABASE_TABLE_H_

#include <string>

#include "base/functional/callback_forward.h"
#include "hns/components/hns_ads/core/internal/account/deposits/deposit_info.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/database/database_table_interface.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"
#include "hns/components/hns_ads/core/public/client/ads_client_callback.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads::database::table {

using GetDepositsCallback =
    base::OnceCallback<void(bool success,
                            const absl::optional<DepositInfo>& deposit)>;

class Deposits final : public TableInterface {
 public:
  void Save(const DepositInfo& deposit, ResultCallback callback);

  void InsertOrUpdate(mojom::DBTransactionInfo* transaction,
                      const CreativeAdList& creative_ads);
  void InsertOrUpdate(mojom::DBTransactionInfo* transaction,
                      const DepositInfo& deposit);

  void GetForCreativeInstanceId(const std::string& creative_instance_id,
                                GetDepositsCallback callback) const;

  void PurgeExpired(ResultCallback callback) const;

  std::string GetTableName() const override;

  void Create(mojom::DBTransactionInfo* transaction) override;
  void Migrate(mojom::DBTransactionInfo* transaction, int to_version) override;

 private:
  std::string BuildInsertOrUpdateSql(mojom::DBCommandInfo* command,
                                     const CreativeAdList& creative_ads) const;
  std::string BuildInsertOrUpdateSql(mojom::DBCommandInfo* command,
                                     const DepositInfo& deposit) const;
};

}  // namespace hns_ads::database::table

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_DEPOSITS_DEPOSITS_DATABASE_TABLE_H_
