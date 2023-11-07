/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_PAGE_ACTION_HNS_PAGE_ACTION_ICON_TYPE_H_
#define HNS_BROWSER_UI_PAGE_ACTION_HNS_PAGE_ACTION_ICON_TYPE_H_

#include "chrome/browser/ui/page_action/page_action_icon_type.h"

namespace hns {

#define DECLARE_HNS_PAGE_ACTION_ICON_TYPE(NAME, VALUE) \
  constexpr PageActionIconType NAME = static_cast<PageActionIconType>(VALUE)

// Use negative values so that our values doesn't conflict with upstream values.
DECLARE_HNS_PAGE_ACTION_ICON_TYPE(kUndefinedPageActionIconType, -1);
DECLARE_HNS_PAGE_ACTION_ICON_TYPE(kPlaylistPageActionIconType, -2);

#undef DECLARE_HNS_PAGE_ACTION_ICON_TYPE

}  // namespace hns

#endif  // HNS_BROWSER_UI_PAGE_ACTION_HNS_PAGE_ACTION_ICON_TYPE_H_
