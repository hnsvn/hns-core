/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// Include the corresponding header first since it defines the same macros and
// therefore avoid undef before use.
#include "chrome/browser/ui/views/frame/browser_view.h"

#include "hns/browser/ui/tabs/features.h"
#include "hns/browser/ui/views/frame/hns_browser_view_layout.h"
#include "hns/browser/ui/views/infobars/hns_infobar_container_view.h"
#include "hns/browser/ui/views/side_panel/hns_side_panel.h"
#include "hns/browser/ui/views/tabs/hns_browser_tab_strip_controller.h"
#include "hns/browser/ui/views/tabs/hns_tab_strip.h"
#include "hns/browser/ui/views/tabs/vertical_tab_utils.h"
#include "hns/browser/ui/views/toolbar/hns_toolbar_view.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/views/frame/tab_strip_region_view.h"
#include "chrome/browser/ui/views/side_panel/side_panel.h"

#define InfoBarContainerView HnsInfoBarContainerView
#define BrowserViewLayout HnsBrowserViewLayout
#define ToolbarView HnsToolbarView
#define BrowserTabStripController HnsBrowserTabStripController
#define TabStrip HnsTabStrip
#define SidePanel HnsSidePanel
#define kAlignLeft kHorizontalAlignLeft
#define kAlignRight kHorizontalAlignRight

#include "src/chrome/browser/ui/views/frame/browser_view.cc"

#undef ToolbarView
#undef BrowserTabStripController
#undef TabStrip
#undef BrowserViewLayout
#undef SidePanel
#undef kAlignLeft
#undef kAlignRight
#undef InfoBarContainerView

void BrowserView::SetNativeWindowPropertyForWidget(views::Widget* widget) {
  // Sets a kBrowserWindowKey to given child |widget| so that we can get
  // BrowserView from the |widget|.
  DCHECK(GetWidget());
  DCHECK_EQ(GetWidget(), widget->GetTopLevelWidget())
      << "The |widget| should be child of BrowserView's widget.";

  widget->SetNativeWindowProperty(kBrowserViewKey, this);
}