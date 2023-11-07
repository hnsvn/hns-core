/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_PUBLIC_KEY_ALIAS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_PUBLIC_KEY_ALIAS_H_

#include <string>

#include "base/containers/flat_map.h"

namespace hns_ads {

using PublicKeyMap =
    base::flat_map</*public_key*/ std::string, /*associated_value*/ double>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_PUBLIC_KEY_ALIAS_H_
