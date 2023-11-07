/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/toolbar/hns_bookmark_sub_menu_model.h"
#include "hns/browser/ui/toolbar/hns_recent_tabs_sub_menu_model.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/ui/toolbar/bookmark_sub_menu_model.h"
#include "chrome/grit/generated_resources.h"

#define IDS_NEW_INCOGNITO_WINDOW_OLD IDS_NEW_INCOGNITO_WINDOW
#undef IDS_NEW_INCOGNITO_WINDOW
#define IDS_NEW_INCOGNITO_WINDOW IDS_HNS_NEW_INCOGNITO_WINDOW

#define RecentTabsSubMenuModel HnsRecentTabsSubMenuModel
#define BookmarkSubMenuModel HnsBookmarkSubMenuModel
#include "src/chrome/browser/ui/toolbar/app_menu_model.cc"
#undef BookmarkSubMenuModel
#undef RecentTabsSubMenuModel

#undef IDS_NEW_INCOGNITO_WINDOW
#define IDS_NEW_INCOGNITO_WINDOW IDS_NEW_INCOGNITO_WINDOW_OLD
