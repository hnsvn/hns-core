/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_COMPONENT_UPDATER_BROWSER_HNS_ON_DEMAND_UPDATER_H_
#define HNS_COMPONENTS_HNS_COMPONENT_UPDATER_BROWSER_HNS_ON_DEMAND_UPDATER_H_

#include <string>

#include "base/functional/callback.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace hns_component_updater {

class HnsOnDemandUpdater {
 public:
  using Callback = base::RepeatingCallback<void(const std::string&)>;
  static HnsOnDemandUpdater* GetInstance();

  HnsOnDemandUpdater(const HnsOnDemandUpdater&) = delete;
  HnsOnDemandUpdater& operator=(const HnsOnDemandUpdater&) = delete;
  ~HnsOnDemandUpdater();
  void OnDemandUpdate(const std::string& id);

  void RegisterOnDemandUpdateCallback(Callback callback);

 private:
  friend base::NoDestructor<HnsOnDemandUpdater>;
  HnsOnDemandUpdater();

  Callback on_demand_update_callback_;
};

}  // namespace hns_component_updater

#endif  // HNS_COMPONENTS_HNS_COMPONENT_UPDATER_BROWSER_HNS_ON_DEMAND_UPDATER_H_
