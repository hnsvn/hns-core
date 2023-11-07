/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/request_otr/common/features.h"

#include "base/feature_list.h"

namespace request_otr {
namespace features {

// When enabled, Hns will block domains listed in the Hns-maintained
// request-OTR ('off-the-record') list, or any server that responds with a
// 'X-Request-OTR: 1' HTTP header, and present a security interstitial with
// choice to proceed normally or in an off-the-record context.
BASE_FEATURE(kHnsRequestOTRTab,
             "HnsRequestOTRTab",
             base::FEATURE_DISABLED_BY_DEFAULT);

}  // namespace features
}  // namespace request_otr
