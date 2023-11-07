/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_COMMAND_LINE_SWITCH_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_COMMAND_LINE_SWITCH_UTIL_H_

#include <string>

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_command_line_switch_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

void InitializeCommandLineSwitches();
void ShutdownCommandLineSwitches();

absl::optional<bool>& DidAppendCommandLineSwitches();
void AppendCommandLineSwitches(
    const CommandLineSwitchList& command_line_switches);

std::string SanitizeCommandLineSwitch(
    const CommandLineSwitchInfo& command_line_switch);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_UNITTEST_UNITTEST_COMMAND_LINE_SWITCH_UTIL_H_
