/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_SHIELDS_HTTPS_EVERYWHERE_COMPONENT_INSTALLER_H_
#define HNS_BROWSER_HNS_SHIELDS_HTTPS_EVERYWHERE_COMPONENT_INSTALLER_H_

#include <string>

namespace component_updater {
class ComponentUpdateService;
}  // namespace component_updater

namespace hns_shields {

void SetHTTPSEverywhereComponentIdAndBase64PublicKeyForTest(
    const std::string& component_id,
    const std::string& component_base64_public_key);

void RegisterHTTPSEverywhereComponent(
    component_updater::ComponentUpdateService* cus);

}  // namespace hns_shields

#endif  // HNS_BROWSER_HNS_SHIELDS_HTTPS_EVERYWHERE_COMPONENT_INSTALLER_H_
