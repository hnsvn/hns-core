/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_SKUS_COMMON_FEATURES_H_
#define HNS_COMPONENTS_SKUS_COMMON_FEATURES_H_

#include "base/feature_list.h"

namespace skus {
namespace features {

// If enabled, this will expose JavaScript methods to domains to pages in a
// Hns curated allow-list. The first use-case is on `account.hns.com`
// where the browser can participate in the credentialing process. When the
// page calls the methods (from JavaScript), it'll invoke the browser's
// implementation which allows for safe credential interception.
BASE_DECLARE_FEATURE(kSkusFeature);

}  // namespace features
}  // namespace skus

#endif  // HNS_COMPONENTS_SKUS_COMMON_FEATURES_H_
