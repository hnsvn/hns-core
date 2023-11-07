/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_TOR_HNS_TOR_PLUGGABLE_TRANSPORT_UPDATER_H_
#define HNS_COMPONENTS_TOR_HNS_TOR_PLUGGABLE_TRANSPORT_UPDATER_H_

#include <string>

#include "base/files/file_path.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/observer_list_types.h"
#include "base/task/sequenced_task_runner.h"
#include "hns/components/hns_component_updater/browser/hns_component.h"

class PrefService;

using hns_component_updater::HnsComponent;

namespace tor {

extern const char kTorPluggableTransportComponentId[];
extern const char kSnowflakeExecutableName[];
extern const char kObfs4ExecutableName[];

class HnsTorPluggableTransportUpdater : public HnsComponent {
 public:
  class Observer : public base::CheckedObserver {
   public:
    virtual void OnPluggableTransportReady(bool success) {}

   protected:
    ~Observer() override = default;
  };

  HnsTorPluggableTransportUpdater(
      HnsComponent::Delegate* component_delegate,
      PrefService* local_state,
      const base::FilePath& user_data_dir);
  HnsTorPluggableTransportUpdater(const HnsTorPluggableTransportUpdater&) =
      delete;
  HnsTorPluggableTransportUpdater& operator=(
      const HnsTorPluggableTransportUpdater&) = delete;
  ~HnsTorPluggableTransportUpdater() override;

  void Register();
  void Unregister();
  void Cleanup();

  bool IsReady() const;
  const base::FilePath& GetSnowflakeExecutable() const;
  const base::FilePath& GetObfs4Executable() const;

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

 protected:
  void OnComponentReady(const std::string& component_id,
                        const base::FilePath& install_dir,
                        const std::string& manifest) override;

 private:
  void OnInitialized(const base::FilePath& install_dir, bool succeeded);

  bool registered_ = false;
  bool is_ready_ = false;

  base::ObserverList<Observer> observers_;
  raw_ptr<PrefService> local_state_ = nullptr;
  base::FilePath user_data_dir_;
  base::FilePath snowflake_path_;  // Relative to the user data dir
  base::FilePath obfs4_path_;      // Relative to the user data dir

  base::WeakPtrFactory<HnsTorPluggableTransportUpdater> weak_ptr_factory_{
      this};
};

}  // namespace tor

#endif  // HNS_COMPONENTS_TOR_HNS_TOR_PLUGGABLE_TRANSPORT_UPDATER_H_
