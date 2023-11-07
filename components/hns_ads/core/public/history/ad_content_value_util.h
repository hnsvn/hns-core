/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_HISTORY_AD_CONTENT_VALUE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_HISTORY_AD_CONTENT_VALUE_UTIL_H_

#include "base/values.h"

namespace hns_ads {

struct AdContentInfo;

base::Value::Dict AdContentToValue(const AdContentInfo& ad_content);
AdContentInfo AdContentFromValue(const base::Value::Dict& dict);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_HISTORY_AD_CONTENT_VALUE_UTIL_H_
