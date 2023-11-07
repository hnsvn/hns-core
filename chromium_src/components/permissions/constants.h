/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_CONSTANTS_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_CONSTANTS_H_

#define kChooserBluetoothOverviewURL kChooserBluetoothOverviewURL_ChromeOverride
#include "src/components/permissions/constants.h"  // IWYU pragma: export
#undef kChooserBluetoothOverviewURL

namespace permissions {
COMPONENT_EXPORT(PERMISSIONS_COMMON)
extern const char kChooserBluetoothOverviewURL[];
}  // namespace permissions

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_CONSTANTS_H_
