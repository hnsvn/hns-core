/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_ANTI_TARGETING_RESOURCE_ANTI_TARGETING_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_ANTI_TARGETING_RESOURCE_ANTI_TARGETING_INFO_H_

#include <map>
#include <set>
#include <string>

#include "base/types/expected.h"
#include "base/values.h"

class GURL;

namespace hns_ads {

using AntiTargetingSiteList = std::set<GURL>;
using AntiTargetingMap =
    std::map</*creative_set_id*/ std::string, AntiTargetingSiteList>;

struct AntiTargetingInfo final {
  AntiTargetingInfo();

  AntiTargetingInfo(const AntiTargetingInfo&) = delete;
  AntiTargetingInfo& operator=(const AntiTargetingInfo&) = delete;

  AntiTargetingInfo(AntiTargetingInfo&&) noexcept;
  AntiTargetingInfo& operator=(AntiTargetingInfo&&) noexcept;

  ~AntiTargetingInfo();

  static base::expected<AntiTargetingInfo, std::string> CreateFromValue(
      base::Value::Dict dict);

  int version = 0;
  AntiTargetingMap sites;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_ANTI_TARGETING_RESOURCE_ANTI_TARGETING_INFO_H_
