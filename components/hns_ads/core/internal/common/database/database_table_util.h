/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_DATABASE_DATABASE_TABLE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_DATABASE_DATABASE_TABLE_UTIL_H_

#include <string>
#include <vector>

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

namespace hns_ads::database {

void CreateTableIndex(mojom::DBTransactionInfo* transaction,
                      const std::string& table_name,
                      const std::string& key);

void DropTable(mojom::DBTransactionInfo* transaction,
               const std::string& table_name);

void DeleteTable(mojom::DBTransactionInfo* transaction,
                 const std::string& table_name);

void CopyTableColumns(mojom::DBTransactionInfo* transaction,
                      const std::string& from,
                      const std::string& to,
                      const std::vector<std::string>& from_columns,
                      const std::vector<std::string>& to_columns,
                      bool should_drop);

void CopyTableColumns(mojom::DBTransactionInfo* transaction,
                      const std::string& from,
                      const std::string& to,
                      const std::vector<std::string>& columns,
                      bool should_drop);

void RenameTable(mojom::DBTransactionInfo* transaction,
                 const std::string& from,
                 const std::string& to);

}  // namespace hns_ads::database

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_DATABASE_DATABASE_TABLE_UTIL_H_
