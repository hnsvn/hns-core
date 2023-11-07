/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/settings/hns_relaunch_handler_mac.h"

#import "hns/browser/mac/sparkle_glue.h"

namespace hns_relaunch_handler {

void RelaunchOnMac() {
  [[SparkleGlue sharedSparkleGlue] relaunch];
}

}  // namespace hns_relaunch_handler