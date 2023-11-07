/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_ADD_ITEM_BUBBLE_DELEGATE_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_ADD_ITEM_BUBBLE_DELEGATE_VIEW_H_

#include <memory>

#include "base/memory/raw_ptr.h"
#include "ui/views/bubble/bubble_dialog_delegate_view.h"

class HnsBrowser;

namespace sidebar {
struct SidebarItem;
}  // namespace sidebar

// TODO(simonhong): Need to apply UI design spec. Currently, this just works.
class SidebarAddItemBubbleDelegateView
    : public views::BubbleDialogDelegateView {
 public:
  METADATA_HEADER(SidebarAddItemBubbleDelegateView);

  static views::Widget* Create(HnsBrowser* browser, views::View* anchor_view);

  ~SidebarAddItemBubbleDelegateView() override;

  SidebarAddItemBubbleDelegateView(const SidebarAddItemBubbleDelegateView&) =
      delete;
  SidebarAddItemBubbleDelegateView& operator=(
      const SidebarAddItemBubbleDelegateView&) = delete;

 private:
  SidebarAddItemBubbleDelegateView(HnsBrowser* browser,
                                   views::View* anchor_view);

  void AddChildViews();

  // Passed |item| will be added to sidebar.
  void OnDefaultItemsButtonPressed(const sidebar::SidebarItem& item);
  void OnCurrentItemButtonPressed();
  void CloseOrReLayoutAfterAddingItem();

  raw_ptr<HnsBrowser> browser_ = nullptr;
};

#endif  // HNS_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_ADD_ITEM_BUBBLE_DELEGATE_VIEW_H_
