/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_component_updater/browser/hns_on_demand_updater.h"

#include <string>

#include "base/no_destructor.h"

namespace hns_component_updater {

HnsOnDemandUpdater* HnsOnDemandUpdater::GetInstance() {
  static base::NoDestructor<HnsOnDemandUpdater> instance;
  return instance.get();
}

HnsOnDemandUpdater::HnsOnDemandUpdater() = default;

HnsOnDemandUpdater::~HnsOnDemandUpdater() = default;

void HnsOnDemandUpdater::OnDemandUpdate(const std::string& id) {
  DCHECK(!on_demand_update_callback_.is_null());
  on_demand_update_callback_.Run(id);
}

void HnsOnDemandUpdater::RegisterOnDemandUpdateCallback(Callback callback) {
  on_demand_update_callback_ = callback;
}


}  // namespace hns_component_updater
