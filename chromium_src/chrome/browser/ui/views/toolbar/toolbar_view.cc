/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/profiles/hns_avatar_toolbar_button.h"
#include "extensions/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "hns/browser/ui/views/location_bar/hns_location_bar_view.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"

#define LocationBarView HnsLocationBarView
#endif

#define HNS_TOOLBAR_VIEW_INIT                                   \
  avatar_ = container_view_->AddChildView(                        \
      std::make_unique<HnsAvatarToolbarButton>(browser_view_)); \
  if (false)

#include "src/chrome/browser/ui/views/toolbar/toolbar_view.cc"
#undef HNS_TOOLBAR_VIEW_INIT
#if BUILDFLAG(ENABLE_EXTENSIONS)
#undef LocationBarView
#endif
