/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_PROMOTION_PROMOTION_UTIL_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_PROMOTION_PROMOTION_UTIL_H_

#include <memory>
#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/mojom_structs.h"

namespace hns_rewards::internal {
namespace promotion {

std::string ParseOSToString(mojom::OperatingSystem os);

std::string ParseClientInfoToString(mojom::ClientInfoPtr info);

mojom::PromotionType ConvertStringToPromotionType(const std::string& type);

mojom::ReportType ConvertPromotionTypeToReportType(
    const mojom::PromotionType type);

}  // namespace promotion
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_PROMOTION_PROMOTION_UTIL_H_
