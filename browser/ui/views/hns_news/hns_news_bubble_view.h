// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_VIEWS_HNS_NEWS_HNS_NEWS_BUBBLE_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_HNS_NEWS_HNS_NEWS_BUBBLE_VIEW_H_

#include "base/memory/raw_ptr.h"
#include "content/public/browser/web_contents.h"
#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/gfx/geometry/size.h"
#include "ui/views/bubble/bubble_dialog_delegate_view.h"
#include "ui/views/controls/label.h"
#include "ui/views/view.h"
#include "ui/views/widget/widget.h"

class HnsNewsFeedsContainerView;

class HnsNewsBubbleView : public views::BubbleDialogDelegateView {
 public:
  static base::WeakPtr<views::Widget> Show(views::View* anchor,
                                           content::WebContents* contents);

  METADATA_HEADER(HnsNewsBubbleView);
  explicit HnsNewsBubbleView(views::View* action_view,
                               content::WebContents* contents);
  HnsNewsBubbleView(const HnsNewsBubbleView&) = delete;
  HnsNewsBubbleView& operator=(const HnsNewsBubbleView&) = delete;
  ~HnsNewsBubbleView() override;

  void OpenManageFeeds();

  // views::BubbleDialogDelegateView:
  void OnThemeChanged() override;

 private:
  raw_ptr<content::WebContents> contents_;
  raw_ptr<views::Label> title_label_ = nullptr;
  raw_ptr<views::Label> subtitle_label_ = nullptr;
  raw_ptr<HnsNewsFeedsContainerView> feeds_container_ = nullptr;
};

#endif  // HNS_BROWSER_UI_VIEWS_HNS_NEWS_HNS_NEWS_BUBBLE_VIEW_H_
