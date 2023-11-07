/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/renderer_context_menu/hns_render_view_context_menu_views.h"

HnsRenderViewContextMenuViews::HnsRenderViewContextMenuViews(
    content::RenderFrameHost& render_frame_host,
    const content::ContextMenuParams& params)
    : RenderViewContextMenuViews(render_frame_host, params) {}

HnsRenderViewContextMenuViews::~HnsRenderViewContextMenuViews() = default;

// static
RenderViewContextMenuViews* HnsRenderViewContextMenuViews::Create(
    content::RenderFrameHost& render_frame_host,
    const content::ContextMenuParams& params) {
  return new HnsRenderViewContextMenuViews(render_frame_host, params);
}

void HnsRenderViewContextMenuViews::Show() {
  // Removes duplicated separator if any. The duplicated separator may appear
  // in |HnsRenderViewContextMenu::InitMenu| after remove the translate menu
  // item.
  RemoveAdjacentSeparators();
  RenderViewContextMenuViews::Show();
}
