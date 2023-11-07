/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SHIELDS_BROWSER_HTTPS_EVERYWHERE_COMPONENT_INSTALLER_H_
#define HNS_COMPONENTS_HNS_SHIELDS_BROWSER_HTTPS_EVERYWHERE_COMPONENT_INSTALLER_H_

#include "base/files/file_path.h"
#include "base/functional/callback.h"

namespace component_updater {
class ComponentUpdateService;
}  // namespace component_updater

namespace hns_shields {

using OnComponentReadyCallback =
    base::RepeatingCallback<void(const base::FilePath& install_path)>;

void RegisterHTTPSEverywhereComponent(
    component_updater::ComponentUpdateService* cus,
    OnComponentReadyCallback callback);

}  // namespace hns_shields

#endif  // HNS_COMPONENTS_HNS_SHIELDS_BROWSER_HTTPS_EVERYWHERE_COMPONENT_INSTALLER_H_
