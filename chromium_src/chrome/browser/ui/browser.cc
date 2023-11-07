/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/hns_browser.h"
#include "hns/browser/ui/hns_browser_command_controller.h"
#include "hns/browser/ui/hns_browser_content_setting_bubble_model_delegate.h"
#include "hns/browser/ui/hns_tab_strip_model_delegate.h"
#include "hns/browser/ui/tabs/hns_tab_strip_model.h"
#include "hns/browser/ui/toolbar/hns_location_bar_model_delegate.h"
#include "chrome/browser/ui/browser_command_controller.h"
#include "chrome/browser/ui/browser_content_setting_bubble_model_delegate.h"

#if !BUILDFLAG(IS_ANDROID)
#include "hns/browser/ui/bookmark/hns_bookmark_tab_helper.h"
#endif

#define HNS_BROWSER_CREATE return new HnsBrowser(params);
#define BrowserContentSettingBubbleModelDelegate \
  HnsBrowserContentSettingBubbleModelDelegate
#define BrowserCommandController HnsBrowserCommandController
#define BrowserLocationBarModelDelegate HnsLocationBarModelDelegate
#if !BUILDFLAG(IS_ANDROID)
#define BookmarkTabHelper HnsBookmarkTabHelper
#endif
#define BrowserTabStripModelDelegate HnsTabStripModelDelegate

#include "src/chrome/browser/ui/browser.cc"

#undef BrowserTabStripModelDelegate
#undef BrowserLocationBarModelDelegate
#undef BrowserContentSettingBubbleModelDelegate
#undef BrowserCommandController
#undef HNS_BROWSER_CREATE

#if !BUILDFLAG(IS_ANDROID)
#undef BookmarkTabHelper
#endif
