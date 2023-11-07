/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_HISTORY_AD_CONTENT_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_HISTORY_AD_CONTENT_UTIL_H_

#include <string>

namespace hns_ads {

class ConfirmationType;
struct AdContentInfo;
struct AdInfo;

AdContentInfo BuildAdContent(const AdInfo& ad,
                             const ConfirmationType& confirmation_type,
                             const std::string& title,
                             const std::string& description);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_HISTORY_AD_CONTENT_UTIL_H_
