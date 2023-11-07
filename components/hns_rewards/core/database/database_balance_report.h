/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_BALANCE_REPORT_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_BALANCE_REPORT_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/database/database_table.h"

namespace hns_rewards::internal {
namespace database {

class DatabaseBalanceReport : public DatabaseTable {
 public:
  explicit DatabaseBalanceReport(RewardsEngineImpl& engine);
  ~DatabaseBalanceReport() override;

  void InsertOrUpdate(mojom::BalanceReportInfoPtr info,
                      LegacyResultCallback callback);

  void InsertOrUpdateList(std::vector<mojom::BalanceReportInfoPtr> list,
                          LegacyResultCallback callback);

  void SetAmount(mojom::ActivityMonth month,
                 int year,
                 mojom::ReportType type,
                 double amount,
                 LegacyResultCallback callback);

  void GetRecord(mojom::ActivityMonth month,
                 int year,
                 mojom::RewardsEngine::GetBalanceReportCallback callback);

  void GetAllRecords(GetBalanceReportListCallback callback);

  void DeleteAllRecords(LegacyResultCallback callback);

 private:
  void OnGetRecord(mojom::RewardsEngine::GetBalanceReportCallback callback,
                   mojom::DBCommandResponsePtr response);

  void OnGetAllRecords(GetBalanceReportListCallback callback,
                       mojom::DBCommandResponsePtr response);
};

}  // namespace database
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_DATABASE_DATABASE_BALANCE_REPORT_H_
