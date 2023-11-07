/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_WALLET_DATA_FILES_INSTALLER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_WALLET_DATA_FILES_INSTALLER_H_

#include <string>

#include "base/functional/callback_forward.h"
#include "base/version.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace component_updater {
class ComponentUpdateService;
}

namespace hns_wallet {

void RegisterWalletDataFilesComponent(
    component_updater::ComponentUpdateService* cus);
void RegisterWalletDataFilesComponentOnDemand(
    component_updater::ComponentUpdateService* cus);
absl::optional<base::Version> GetLastInstalledWalletVersion();
void SetLastInstalledWalletVersionForTest(const base::Version& version);
std::string GetWalletDataFilesComponentId();

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_WALLET_DATA_FILES_INSTALLER_H_
