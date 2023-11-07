/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/update_util.h"

#include "base/command_line.h"
#include "hns/components/constants/hns_switches.h"

namespace hns {

bool UpdateEnabled() {
#if defined(OFFICIAL_BUILD)
  return !base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableHnsUpdate);
#else
  return false;
#endif
}

}  // namespace hns
