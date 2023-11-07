/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DATABASE_DATABASE_MANAGER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DATABASE_DATABASE_MANAGER_H_

#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "hns/components/hns_ads/core/internal/database/database_manager_observer.h"
#include "hns/components/hns_ads/core/public/client/ads_client_callback.h"

namespace hns_ads {

class DatabaseManager final {
 public:
  DatabaseManager();

  DatabaseManager(const DatabaseManager&) = delete;
  DatabaseManager& operator=(const DatabaseManager&) = delete;

  DatabaseManager(DatabaseManager&&) noexcept = delete;
  DatabaseManager& operator=(DatabaseManager&&) noexcept = delete;

  ~DatabaseManager();

  static DatabaseManager& GetInstance();

  void AddObserver(DatabaseManagerObserver* observer);
  void RemoveObserver(DatabaseManagerObserver* observer);

  void CreateOrOpen(ResultCallback callback);

 private:
  void CreateOrOpenCallback(ResultCallback callback,
                            mojom::DBCommandResponseInfoPtr command_response);

  void Create(ResultCallback callback) const;
  void CreateCallback(ResultCallback callback, bool success) const;

  void MaybeMigrate(int from_version, ResultCallback callback) const;
  void MigrateFromVersionCallback(int from_version,
                                  ResultCallback callback,
                                  bool success) const;

  void NotifyWillCreateOrOpenDatabase() const;
  void NotifyDidCreateDatabase() const;
  void NotifyDidOpenDatabase() const;
  void NotifyFailedToCreateOrOpenDatabase() const;
  void NotifyWillMigrateDatabase(int from_version, int to_version) const;
  void NotifyDidMigrateDatabase(int from_version, int to_version) const;
  void NotifyFailedToMigrateDatabase(int from_version, int to_version) const;
  void NotifyDatabaseIsReady() const;

  base::ObserverList<DatabaseManagerObserver> observers_;

  base::WeakPtrFactory<DatabaseManager> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DATABASE_DATABASE_MANAGER_H_
