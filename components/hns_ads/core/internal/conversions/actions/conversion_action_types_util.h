/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_ACTIONS_CONVERSION_ACTION_TYPES_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_ACTIONS_CONVERSION_ACTION_TYPES_UTIL_H_

#include <string>

#include "hns/components/hns_ads/core/internal/conversions/actions/conversion_action_types.h"

namespace hns_ads {

class ConfirmationType;

ConversionActionType ToConversionActionType(
    const ConfirmationType& confirmation_type);

ConversionActionType StringToConversionActionType(
    const std::string& action_type);

std::string ConversionActionTypeToString(ConversionActionType action_type);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_ACTIONS_CONVERSION_ACTION_TYPES_UTIL_H_
