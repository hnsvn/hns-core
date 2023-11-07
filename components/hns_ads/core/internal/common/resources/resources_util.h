/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_RESOURCES_RESOURCES_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_RESOURCES_RESOURCES_UTIL_H_

#include <string>

#include "base/functional/callback.h"
#include "hns/components/hns_ads/core/internal/common/resources/resource_parsing_error_or.h"

namespace hns_ads {

template <typename T>
using LoadAndParseResourceCallback =
    base::OnceCallback<void(ResourceParsingErrorOr<T>)>;

template <typename T>
void LoadAndParseResource(const std::string& id,
                          int version,
                          LoadAndParseResourceCallback<T> callback);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_RESOURCES_RESOURCES_UTIL_H_
