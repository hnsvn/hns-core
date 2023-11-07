// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_VIEWS_HNS_NEWS_HNS_NEWS_FEED_ITEM_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_HNS_NEWS_HNS_NEWS_FEED_ITEM_VIEW_H_

#include <vector>

#include "hns/browser/hns_news/hns_news_tab_helper.h"
#include "ui/views/view.h"

namespace content {
class WebContents;
}

namespace views {
class MdTextButton;
}

class HnsNewsFeedItemView : public views::View,
                              public HnsNewsTabHelper::PageFeedsObserver {
 public:
  METADATA_HEADER(HnsNewsFeedItemView);

  HnsNewsFeedItemView(HnsNewsTabHelper::FeedDetails details,
                        content::WebContents* contents);
  HnsNewsFeedItemView(const HnsNewsFeedItemView&) = delete;
  HnsNewsFeedItemView& operator=(const HnsNewsFeedItemView&) = delete;
  ~HnsNewsFeedItemView() override;

  void Update();
  void OnPressed();

  // HnsNewsTabHelper::PageFeedsObserver:
  void OnAvailableFeedsChanged(
      const std::vector<HnsNewsTabHelper::FeedDetails>& feeds) override;

 private:
  bool loading_ = false;
  raw_ptr<views::MdTextButton> subscribe_button_ = nullptr;

  HnsNewsTabHelper::FeedDetails feed_details_;
  raw_ptr<content::WebContents> contents_;
  raw_ptr<HnsNewsTabHelper> tab_helper_;

  base::ScopedObservation<HnsNewsTabHelper,
                          HnsNewsTabHelper::PageFeedsObserver>
      tab_helper_observation_{this};
};

#endif  // HNS_BROWSER_UI_VIEWS_HNS_NEWS_HNS_NEWS_FEED_ITEM_VIEW_H_
