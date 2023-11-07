/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/common/pref_names.h"

namespace hns_vpn {
namespace prefs {

#if BUILDFLAG(IS_ANDROID)
const char kHnsVPNPurchaseTokenAndroid[] =
    "hns.hns_vpn.purchase_token_android";
const char kHnsVPNPackageAndroid[] = "hns.hns_vpn.package_android";
const char kHnsVPNProductIdAndroid[] = "hns.hns_vpn.product_id_android";
#endif

}  // namespace prefs

}  // namespace hns_vpn
