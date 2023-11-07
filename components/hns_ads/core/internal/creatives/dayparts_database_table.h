/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_DAYPARTS_DATABASE_TABLE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_DAYPARTS_DATABASE_TABLE_H_

#include <string>

#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/database/database_table_interface.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"
#include "hns/components/hns_ads/core/public/client/ads_client_callback.h"

namespace hns_ads::database::table {

class Dayparts final : public TableInterface {
 public:
  void InsertOrUpdate(mojom::DBTransactionInfo* transaction,
                      const CreativeAdList& creative_ads);

  void Delete(ResultCallback callback) const;

  std::string GetTableName() const override;

  void Create(mojom::DBTransactionInfo* transaction) override;
  void Migrate(mojom::DBTransactionInfo* transaction, int to_version) override;

 private:
  std::string BuildInsertOrUpdateSql(mojom::DBCommandInfo* command,
                                     const CreativeAdList& creative_ads) const;
};

}  // namespace hns_ads::database::table

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_DAYPARTS_DATABASE_TABLE_H_
