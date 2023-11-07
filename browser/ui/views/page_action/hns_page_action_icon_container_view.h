/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_PAGE_ACTION_HNS_PAGE_ACTION_ICON_CONTAINER_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_PAGE_ACTION_HNS_PAGE_ACTION_ICON_CONTAINER_VIEW_H_

#include "chrome/browser/ui/views/page_action/page_action_icon_container.h"

class HnsPageActionIconContainerView : public PageActionIconContainerView {
 public:
  METADATA_HEADER(HnsPageActionIconContainerView);

  // Unfortunately, |PageActionIconParams| can't not be copied or moved. So drop
  // const qualifier to modify |param| for Hns.
  explicit HnsPageActionIconContainerView(PageActionIconParams& param);
  ~HnsPageActionIconContainerView() override;
};

#endif  // HNS_BROWSER_UI_VIEWS_PAGE_ACTION_HNS_PAGE_ACTION_ICON_CONTAINER_VIEW_H_
