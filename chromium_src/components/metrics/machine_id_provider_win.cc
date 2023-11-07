/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/command_line.h"
#include "hns/components/constants/hns_switches.h"

namespace {
bool IsMachineIdDisabled() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableMachineId);
}

}  // namespace
#include "src/components/metrics/machine_id_provider_win.cc"
