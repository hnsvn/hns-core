/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_FEDERATED_DATA_STORES_ASYNC_DATA_STORE_H_
#define HNS_COMPONENTS_HNS_FEDERATED_DATA_STORES_ASYNC_DATA_STORE_H_

#include <vector>

#include "base/files/file_path.h"
#include "base/functional/callback.h"
#include "base/threading/sequence_bound.h"
#include "hns/components/hns_federated/data_stores/data_store.h"
#include "hns/components/hns_federated/public/interfaces/hns_federated.mojom.h"

namespace hns_federated {

// Wrapper around DataStore class to handle SequenceBound async logic
class AsyncDataStore {
 public:
  explicit AsyncDataStore(DataStoreTask data_store_task,
                          base::FilePath db_path);
  ~AsyncDataStore();

  AsyncDataStore(const AsyncDataStore&) = delete;
  AsyncDataStore& operator=(const AsyncDataStore&) = delete;

  void InitializeDatabase(base::OnceCallback<void(bool)> callback);

  void AddTrainingInstance(
      std::vector<hns_federated::mojom::CovariateInfoPtr> training_instance,
      base::OnceCallback<void(bool)> callback);
  void LoadTrainingData(base::OnceCallback<void(TrainingData)> callback);
  void PurgeTrainingDataAfterExpirationDate();

 private:
  const base::SequenceBound<DataStore> data_store_;
};

}  // namespace hns_federated

#endif  // HNS_COMPONENTS_HNS_FEDERATED_DATA_STORES_ASYNC_DATA_STORE_H_
