/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_MEDIA_HELPER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_MEDIA_HELPER_H_

#include <string>

namespace hns_rewards::internal {

std::string GetMediaKey(const std::string& mediaId, const std::string& type);

std::string ExtractData(const std::string& data,
                        const std::string& match_after,
                        const std::string& match_until);

}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_MEDIA_HELPER_H_
