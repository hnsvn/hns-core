// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_COMMANDER_HNS_SIMPLE_COMMAND_SOURCE_H_
#define HNS_BROWSER_UI_COMMANDER_HNS_SIMPLE_COMMAND_SOURCE_H_

#include "chrome/browser/ui/commander/command_source.h"

namespace commander {

class HnsSimpleCommandSource : public CommandSource {
 public:
  HnsSimpleCommandSource();
  ~HnsSimpleCommandSource() override;

  HnsSimpleCommandSource(const HnsSimpleCommandSource& other) = delete;
  HnsSimpleCommandSource& operator=(const HnsSimpleCommandSource& other) =
      delete;

  // CommandSource:
  CommandSource::CommandResults GetCommands(const std::u16string& input,
                                            Browser* browser) const override;
};

}  // namespace commander

#endif  // HNS_BROWSER_UI_COMMANDER_HNS_SIMPLE_COMMAND_SOURCE_H_
