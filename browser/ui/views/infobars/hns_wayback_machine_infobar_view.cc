/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/infobars/hns_wayback_machine_infobar_view.h"

#include <string>
#include <utility>

#include "hns/browser/infobars/hns_wayback_machine_delegate_impl.h"
#include "hns/browser/ui/views/infobars/hns_wayback_machine_infobar_contents_view.h"
#include "hns/components/hns_wayback_machine/hns_wayback_machine_infobar_delegate.h"

// static
std::unique_ptr<infobars::InfoBar>
HnsWaybackMachineDelegateImpl::CreateInfoBarView(
    std::unique_ptr<HnsWaybackMachineInfoBarDelegate> delegate,
    content::WebContents* contents) {
  return std::make_unique<HnsWaybackMachineInfoBarView>(std::move(delegate),
                                                          contents);
}

HnsWaybackMachineInfoBarView::HnsWaybackMachineInfoBarView(
    std::unique_ptr<HnsWaybackMachineInfoBarDelegate> delegate,
    content::WebContents* contents)
    : InfoBarView(std::move(delegate)),
      sub_views_(*new HnsWaybackMachineInfoBarContentsView(contents)) {
  sub_views_->SizeToPreferredSize();
  AddChildView(&*sub_views_);
}

HnsWaybackMachineInfoBarView::~HnsWaybackMachineInfoBarView() = default;

void HnsWaybackMachineInfoBarView::Layout() {
  InfoBarView::Layout();
  // |sub_views_| occupies from the beginning.
  // Don't adjust child view's height. Just use their preferred height.
  // It can cause infinite Layout loop because of infobar's height
  // re-calculation during the animation.
  sub_views_->SetBounds(0, OffsetY(&*sub_views_), GetEndX(),
                        sub_views_->height());
}
