/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_

#include "hns/browser/ui/hns_browser_window.h"
#include "hns/browser/ui/views/frame/hns_browser_view_layout.h"
#include "hns/browser/ui/views/side_panel/hns_side_panel.h"
#include "build/build_config.h"
#include "chrome/browser/ui/views/side_panel/side_panel.h"

#define BrowserViewLayoutDelegateImpl \
  BrowserViewLayoutDelegateImpl;      \
  friend class HnsBrowserView;      \
  void SetNativeWindowPropertyForWidget(views::Widget* widget)
#define BrowserWindow HnsBrowserWindow
#define BrowserViewLayout HnsBrowserViewLayout
#define SidePanel HnsSidePanel
#define GetContentsLayoutManager     \
  GetContentsLayoutManager_Unused(); \
  virtual ContentsLayoutManager* GetContentsLayoutManager

#define MaybeShowReadingListInSidePanelIPH \
  virtual MaybeShowReadingListInSidePanelIPH

#define GetTabStripVisible virtual GetTabStripVisible
#define BrowserViewLayout HnsBrowserViewLayout

#define GetTabSearchBubbleHost     \
  GetTabSearchBubbleHost_Unused(); \
  virtual TabSearchBubbleHost* GetTabSearchBubbleHost

#if BUILDFLAG(IS_WIN)
#define GetSupportsTitle virtual GetSupportsTitle

// On Windows <winuser.h> defines LoadAccelerators
#pragma push_macro("LoadAccelerators")
#undef LoadAccelerators
#endif
#define LoadAccelerators virtual LoadAccelerators

#include "src/chrome/browser/ui/views/frame/browser_view.h"  // IWYU pragma: export

#undef LoadAccelerators
#if BUILDFLAG(IS_WIN)
#pragma pop_macro("LoadAccelerators")
#undef GetSupportsTitle
#endif

#undef GetTabSearchBubbleHost
#undef BrowserViewLayout
#undef GetTabStripVisible
#undef BrowserViewLayoutDelegateImpl
#undef BrowserWindow
#undef MaybeShowReadingListInSidePanelIPH
#undef BrowserViewLayout
#undef SidePanel
#undef GetContentsLayoutManager

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_
