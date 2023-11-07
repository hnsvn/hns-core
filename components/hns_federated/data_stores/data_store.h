/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_FEDERATED_DATA_STORES_DATA_STORE_H_
#define HNS_COMPONENTS_HNS_FEDERATED_DATA_STORES_DATA_STORE_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/sequence_checker.h"
#include "hns/components/hns_federated/public/interfaces/hns_federated.mojom.h"
#include "sql/database.h"

namespace hns_federated {

using TrainingData = base::flat_map<int, std::vector<mojom::CovariateInfoPtr>>;

struct DataStoreTask {
  int id = 0;
  const std::string name;
  int max_number_of_records = 0;
  base::TimeDelta max_retention_days;
};

class DataStore {
 public:
  explicit DataStore(const DataStoreTask data_store_task,
                     const base::FilePath& db_file_path);
  ~DataStore();

  DataStore(const DataStore&) = delete;
  DataStore& operator=(const DataStore&) = delete;

  bool InitializeDatabase();

  int GetNextTrainingInstanceId();
  void SaveCovariate(const hns_federated::mojom::CovariateInfo& covariate,
                     int training_instance_id,
                     const base::Time created_at);
  bool AddTrainingInstance(
      const std::vector<hns_federated::mojom::CovariateInfoPtr>
          training_instance);

  bool DeleteTrainingData();
  TrainingData LoadTrainingData();
  void PurgeTrainingDataAfterExpirationDate();

 protected:
  friend class DataStoreTest;

  sql::Database database_;
  base::FilePath db_file_path_;
  DataStoreTask data_store_task_;

 private:
  bool MaybeCreateTable();

  SEQUENCE_CHECKER(sequence_checker_);
};

}  // namespace hns_federated

#endif  // HNS_COMPONENTS_HNS_FEDERATED_DATA_STORES_DATA_STORE_H_
