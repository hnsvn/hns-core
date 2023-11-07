/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/views/infobars/infobar_container_view.h"

#ifndef HNS_BROWSER_UI_VIEWS_INFOBARS_HNS_INFOBAR_CONTAINER_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_INFOBARS_HNS_INFOBAR_CONTAINER_VIEW_H_

class HnsInfoBarContainerView : public InfoBarContainerView {
 public:
  explicit HnsInfoBarContainerView(
      infobars::InfoBarContainer::Delegate* delegate);
  HnsInfoBarContainerView(const HnsInfoBarContainerView&) = delete;
  HnsInfoBarContainerView& operator=(const HnsInfoBarContainerView&) =
      delete;
  ~HnsInfoBarContainerView() override;
};

#endif  // HNS_BROWSER_UI_VIEWS_INFOBARS_HNS_INFOBAR_CONTAINER_VIEW_H_
