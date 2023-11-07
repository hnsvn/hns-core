/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_WALLET_DATA_FILES_INSTALLER_ANDROID_UTIL_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_WALLET_DATA_FILES_INSTALLER_ANDROID_UTIL_H_

#include "build/build_config.h"

static_assert BUILDFLAG(IS_ANDROID);

namespace hns_wallet {

bool IsHnsWalletConfiguredOnAndroid();

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_WALLET_DATA_FILES_INSTALLER_ANDROID_UTIL_H_
