/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_VIEWS_DELEGATE_LINUX_H_
#define HNS_BROWSER_UI_VIEWS_HNS_VIEWS_DELEGATE_LINUX_H_

#include "chrome/browser/ui/views/chrome_views_delegate.h"

class HnsViewsDelegateLinux : public ChromeViewsDelegate {
 public:
  HnsViewsDelegateLinux() = default;
  HnsViewsDelegateLinux(const HnsViewsDelegateLinux&) = delete;
  HnsViewsDelegateLinux& operator=(const HnsViewsDelegateLinux&) = delete;
  ~HnsViewsDelegateLinux() override = default;
 private:
  // ChromeViewsDelegate overrides:
  gfx::ImageSkia* GetDefaultWindowIcon() const override;
};

#endif  // HNS_BROWSER_UI_VIEWS_HNS_VIEWS_DELEGATE_LINUX_H_
