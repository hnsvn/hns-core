/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/flags/debug/debug_command_line_switch_parser_util.h"

#include "hns/components/hns_rewards/common/rewards_flags.h"

namespace hns_ads {

bool ParseDebugCommandLineSwitch() {
  return hns_rewards::RewardsFlags::ForCurrentProcess().debug;
}

}  // namespace hns_ads
