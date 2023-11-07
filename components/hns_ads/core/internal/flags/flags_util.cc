/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/flags/flags_util.h"

#include "hns/components/hns_ads/core/internal/flags/debug/debug_command_line_switch_parser_util.h"
#include "hns/components/hns_ads/core/internal/flags/did_override/did_override_command_line_switch_values_util.h"
#include "hns/components/hns_ads/core/internal/flags/did_override/did_override_command_line_switches_util.h"
#include "hns/components/hns_ads/core/internal/flags/did_override/did_override_features_from_command_line_util.h"
#include "hns/components/hns_ads/core/internal/flags/environment/environment_command_line_switch_parser_util.h"
#include "hns/components/hns_ads/core/internal/flags/flag_constants.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

namespace {

mojom::EnvironmentType ChooseEnvironmentType() {
  return ParseEnvironmentCommandLineSwitch().value_or(kDefaultEnvironmentType);
}

}  // namespace

mojom::FlagsPtr BuildFlags() {
  mojom::FlagsPtr flags = mojom::Flags::New();

  flags->should_debug = ParseDebugCommandLineSwitch();

  flags->did_override_from_command_line =
      DidOverrideFeaturesFromCommandLine() ||
      DidOverrideCommandLineSwitchValues() || DidOverrideCommandLineSwitches();

  flags->environment_type = ChooseEnvironmentType();

  return flags;
}

}  // namespace hns_ads
