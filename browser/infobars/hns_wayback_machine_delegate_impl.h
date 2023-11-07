/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_INFOBARS_HNS_WAYBACK_MACHINE_DELEGATE_IMPL_H_
#define HNS_BROWSER_INFOBARS_HNS_WAYBACK_MACHINE_DELEGATE_IMPL_H_

#include <memory>

#include "hns/components/hns_wayback_machine/hns_wayback_machine_delegate.h"

namespace content {
class WebContents;
}  // namespace content

namespace infobars {
class InfoBar;
}  // namespace infobars

class HnsWaybackMachineDelegateImpl : public HnsWaybackMachineDelegate {
 public:
  static void AttachTabHelperIfNeeded(content::WebContents* web_contents);

  HnsWaybackMachineDelegateImpl();
  ~HnsWaybackMachineDelegateImpl() override;

  HnsWaybackMachineDelegateImpl(
      const HnsWaybackMachineDelegateImpl&) = delete;
  HnsWaybackMachineDelegateImpl& operator=(
      const HnsWaybackMachineDelegateImpl&) = delete;

 private:
  // HnsWaybackMachineDelegate overrides:
  void CreateInfoBar(content::WebContents* web_contents) override;

  std::unique_ptr<infobars::InfoBar> CreateInfoBarView(
      std::unique_ptr<HnsWaybackMachineInfoBarDelegate> delegate,
      content::WebContents* contents);
};

#endif  // HNS_BROWSER_INFOBARS_HNS_WAYBACK_MACHINE_DELEGATE_IMPL_H_
