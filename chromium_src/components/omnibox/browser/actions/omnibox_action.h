// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_OMNIBOX_BROWSER_ACTIONS_OMNIBOX_ACTION_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_OMNIBOX_BROWSER_ACTIONS_OMNIBOX_ACTION_H_

#include "hns/components/commander/common/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_COMMANDER)
#include "hns/components/commander/browser/commander_frontend_delegate.h"

#define OpenSharingHub  \
  OpenSharingHub() = 0; \
  virtual commander::CommanderFrontendDelegate* GetCommanderDelegate
#endif

#include "src/components/omnibox/browser/actions/omnibox_action.h"  // IWYU pragma: export
#undef OpenSharingHub

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_OMNIBOX_BROWSER_ACTIONS_OMNIBOX_ACTION_H_
