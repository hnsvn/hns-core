/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_BAT_HELPER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_BAT_HELPER_H_

#include <string>
#include <vector>

namespace hns_rewards::internal {

bool getJSONValue(const std::string& field_name,
                  const std::string& json,
                  std::string* value);

std::string getBase64(const std::vector<uint8_t>& in);

// Sign using ed25519 algorithm
std::string sign(const std::vector<std::string>& keys,
                 const std::vector<std::string>& values,
                 const std::string& key_id,
                 const std::vector<uint8_t>& secretKey);

bool HasSameDomainAndPath(const std::string& url,
                          const std::string& to_match,
                          const std::string& path);

}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_BAT_HELPER_H_
