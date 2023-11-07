// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/components/hns_news/browser/urls.h"

#include <string>

#include "base/command_line.h"
#include "hns/components/hns_news/common/switches.h"

namespace hns_news {

std::string GetHostname() {
  std::string from_switch =
      base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
          switches::kHnsNewsHost);
  if (from_switch.empty()) {
    return "hns-today-cdn.hns.com";
  } else {
    return from_switch;
  }
}

}  // namespace hns_news
