/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_RESOURCE_PROVIDER_H_
#define HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_RESOURCE_PROVIDER_H_

#include <string>

#include "base/functional/callback.h"
#include "base/observer_list.h"
#include "base/observer_list_types.h"
#include "hns/components/hns_component_updater/browser/dat_file_util.h"

using hns_component_updater::DATFileDataBuffer;

namespace hns_shields {

// Interface for any source that can load resource replacements into an adblock
// engine.
class AdBlockResourceProvider {
 public:
  class Observer : public base::CheckedObserver {
   public:
    virtual void OnResourcesLoaded(const std::string& resources_json) = 0;
  };

  AdBlockResourceProvider();
  virtual ~AdBlockResourceProvider();

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  virtual void LoadResources(
      base::OnceCallback<void(const std::string& resources_json)>) = 0;

 protected:
  void OnResourcesLoaded(const std::string& resources_json);

 private:
  base::ObserverList<Observer> observers_;
};

}  // namespace hns_shields

#endif  // HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_RESOURCE_PROVIDER_H_
