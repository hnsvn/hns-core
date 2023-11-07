/* Copyright 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/permissions/constants.h"

#define kChooserBluetoothOverviewURL kChooserBluetoothOverviewURL_ChromeOverride
#include "src/components/permissions/constants.cc"
#undef kChooserBluetoothOverviewURL

namespace permissions {

const char kChooserBluetoothOverviewURL[] =
    "https://github.com/hnsvn/hns-browser/wiki/Web-API-Permissions";

}  // namespace permissions
