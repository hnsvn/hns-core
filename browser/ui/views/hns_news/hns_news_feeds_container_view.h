// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_VIEWS_HNS_NEWS_HNS_NEWS_FEEDS_CONTAINER_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_HNS_NEWS_HNS_NEWS_FEEDS_CONTAINER_VIEW_H_

#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/gfx/geometry/size.h"
#include "ui/views/layout/layout_types.h"
#include "ui/views/view.h"

namespace content {
class WebContents;
}

class HnsNewsFeedsContainerView : public views::View {
 public:
  METADATA_HEADER(HnsNewsFeedsContainerView);

  explicit HnsNewsFeedsContainerView(content::WebContents* contents);
  HnsNewsFeedsContainerView(const HnsNewsFeedsContainerView&) = delete;
  HnsNewsFeedsContainerView& operator=(const HnsNewsFeedsContainerView&) =
      delete;
  ~HnsNewsFeedsContainerView() override;

  // views::View
  void OnThemeChanged() override;
};

#endif  // HNS_BROWSER_UI_VIEWS_HNS_NEWS_HNS_NEWS_FEEDS_CONTAINER_VIEW_H_
