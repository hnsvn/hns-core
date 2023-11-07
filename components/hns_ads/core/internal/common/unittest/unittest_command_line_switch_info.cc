/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_command_line_switch_info.h"

#include <utility>

namespace hns_ads {

CommandLineSwitchInfo::CommandLineSwitchInfo() = default;

CommandLineSwitchInfo::CommandLineSwitchInfo(std::string key, std::string value)
    : key(std::move(key)), value(std::move(value)) {}

}  // namespace hns_ads
