/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_INFOBARS_HNS_WAYBACK_MACHINE_INFOBAR_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_INFOBARS_HNS_WAYBACK_MACHINE_INFOBAR_VIEW_H_

#include <memory>

#include "base/memory/raw_ref.h"
#include "chrome/browser/ui/views/infobars/infobar_view.h"

namespace content {
class WebContents;
}

class HnsWaybackMachineInfoBarDelegate;

class HnsWaybackMachineInfoBarView : public InfoBarView {
 public:
  HnsWaybackMachineInfoBarView(
      std::unique_ptr<HnsWaybackMachineInfoBarDelegate> delegate,
      content::WebContents* contents);
  ~HnsWaybackMachineInfoBarView() override;

  HnsWaybackMachineInfoBarView(
      const HnsWaybackMachineInfoBarView&) = delete;
  HnsWaybackMachineInfoBarView& operator=(
      const HnsWaybackMachineInfoBarView&) = delete;

 private:
  // InfoBarView overrides:
  void Layout() override;

  const raw_ref<views::View> sub_views_;
};

#endif  // HNS_BROWSER_UI_VIEWS_INFOBARS_HNS_WAYBACK_MACHINE_INFOBAR_VIEW_H_
