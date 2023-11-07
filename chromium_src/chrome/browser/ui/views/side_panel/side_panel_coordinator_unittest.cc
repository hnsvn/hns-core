// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "chrome/browser/ui/views/side_panel/side_panel_coordinator.h"

#include "hns/browser/ui/views/side_panel/hns_side_panel.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/side_panel/side_panel.h"

#define kAlignLeft kHorizontalAlignLeft
#define kAlignRight kHorizontalAlignRight
#define SidePanel HnsSidePanel
#include "src/chrome/browser/ui/views/side_panel/side_panel_coordinator_unittest.cc"
#undef SidePanel
#undef kAlignLeft
#undef kAlignRight
