/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/flags/environment/environment_command_line_switch_parser_util.h"

#include "base/notreached.h"
#include "hns/components/hns_rewards/common/rewards_flags.h"

namespace hns_ads {

absl::optional<mojom::EnvironmentType> ParseEnvironmentCommandLineSwitch() {
  const hns_rewards::RewardsFlags& flags =
      hns_rewards::RewardsFlags::ForCurrentProcess();
  if (!flags.environment) {
    return absl::nullopt;
  }

  switch (*flags.environment) {
    case hns_rewards::RewardsFlags::Environment::kDevelopment:
    case hns_rewards::RewardsFlags::Environment::kStaging: {
      return mojom::EnvironmentType::kStaging;
    }

    case hns_rewards::RewardsFlags::Environment::kProduction: {
      return mojom::EnvironmentType::kProduction;
    }
  }

  NOTREACHED_NORETURN() << "Unexpected value for Environment: "
                        << static_cast<int>(*flags.environment);
}

}  // namespace hns_ads
