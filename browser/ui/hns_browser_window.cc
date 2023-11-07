/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/hns_browser_window.h"

#include <vector>

// Provide a base implementation (important for `TestBrowserWindow ` in tests)
// For real implementation, see `HnsBrowserView`.

speedreader::SpeedreaderBubbleView* HnsBrowserWindow::ShowSpeedreaderBubble(
    speedreader::SpeedreaderTabHelper* tab_helper,
    speedreader::SpeedreaderBubbleLocation location) {
  return nullptr;
}

gfx::Rect HnsBrowserWindow::GetShieldsBubbleRect() {
  return gfx::Rect();
}

// static
HnsBrowserWindow* HnsBrowserWindow::From(BrowserWindow* window) {
  return static_cast<HnsBrowserWindow*>(window);
}

#if defined(TOOLKIT_VIEWS)
sidebar::Sidebar* HnsBrowserWindow::InitSidebar() {
  return nullptr;
}

void HnsBrowserWindow::ToggleSidebar() {}

bool HnsBrowserWindow::HasSelectedURL() const {
  return false;
}
void HnsBrowserWindow::CleanAndCopySelectedURL() {}

#endif
