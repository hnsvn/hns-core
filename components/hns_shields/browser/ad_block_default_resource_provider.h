/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_DEFAULT_RESOURCE_PROVIDER_H_
#define HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_DEFAULT_RESOURCE_PROVIDER_H_

#include <string>

#include "base/functional/callback.h"
#include "base/timer/timer.h"
#include "hns/components/hns_shields/browser/ad_block_resource_provider.h"

namespace component_updater {
class ComponentUpdateService;
}  // namespace component_updater

namespace base {
class FilePath;
}

namespace hns_shields {

class AdBlockDefaultResourceProvider : public AdBlockResourceProvider {
 public:
  explicit AdBlockDefaultResourceProvider(
      component_updater::ComponentUpdateService* cus);
  ~AdBlockDefaultResourceProvider() override;
  AdBlockDefaultResourceProvider(const AdBlockDefaultResourceProvider&) =
      delete;
  AdBlockDefaultResourceProvider& operator=(
      const AdBlockDefaultResourceProvider&) = delete;

  void LoadResources(
      base::OnceCallback<void(const std::string& resources_json)>) override;

 private:
  void OnComponentReady(const base::FilePath&);

  base::FilePath component_path_;
  base::RepeatingTimer update_check_timer_;

  base::WeakPtrFactory<AdBlockDefaultResourceProvider> weak_factory_{this};
};

}  // namespace hns_shields

#endif  // HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_DEFAULT_RESOURCE_PROVIDER_H_
