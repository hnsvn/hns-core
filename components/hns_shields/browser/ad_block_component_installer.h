// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_COMPONENT_INSTALLER_H_
#define HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_COMPONENT_INSTALLER_H_

#include <string>

#include "base/files/file_path.h"
#include "base/functional/callback.h"

namespace component_updater {
class ComponentUpdateService;
}  // namespace component_updater

namespace hns_shields {

using OnComponentReadyCallback =
    base::RepeatingCallback<void(const base::FilePath& install_path)>;

void RegisterAdBlockDefaultComponent(
    component_updater::ComponentUpdateService* cus,
    OnComponentReadyCallback callback);

void RegisterAdBlockDefaultResourceComponent(
    component_updater::ComponentUpdateService* cus,
    OnComponentReadyCallback callback);
void CheckAdBlockComponentsUpdate();

void RegisterAdBlockFilterListCatalogComponent(
    component_updater::ComponentUpdateService* cus,
    OnComponentReadyCallback callback);

void RegisterAdBlockFiltersComponent(
    component_updater::ComponentUpdateService* cus,
    const std::string& component_public_key,
    const std::string& component_id,
    const std::string& component_name,
    OnComponentReadyCallback callback);

}  // namespace hns_shields

#endif  // HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_COMPONENT_INSTALLER_H_
