/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_CONSTANTS_HNS_SERVICES_KEY_HELPER_H_
#define HNS_COMPONENTS_CONSTANTS_HNS_SERVICES_KEY_HELPER_H_

class GURL;

namespace hns {

bool ShouldAddHnsServicesKeyHeader(const GURL& url);

}  // namespace hns

#endif  // HNS_COMPONENTS_CONSTANTS_HNS_SERVICES_KEY_HELPER_H_
