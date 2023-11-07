// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_COMMANDER_TAB_COMMAND_SOURCE_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_COMMANDER_TAB_COMMAND_SOURCE_H_

#include "src/chrome/browser/ui/commander/tab_command_source.h"  // IWYU pragma: export

#include "chrome/browser/ui/commander/command_source.h"

namespace commander {

class HnsTabCommandSource : public CommandSource {
 public:
  HnsTabCommandSource();
  ~HnsTabCommandSource() override;

  HnsTabCommandSource(const HnsTabCommandSource& other) = delete;
  HnsTabCommandSource& operator=(const HnsTabCommandSource& other) = delete;

  // Command source overrides
  CommandSource::CommandResults GetCommands(const std::u16string& input,
                                            Browser* browser) const override;
};

}  // namespace commander

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_COMMANDER_TAB_COMMAND_SOURCE_H_
