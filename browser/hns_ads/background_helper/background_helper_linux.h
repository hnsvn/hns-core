/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_BACKGROUND_HELPER_BACKGROUND_HELPER_LINUX_H_
#define HNS_BROWSER_HNS_ADS_BACKGROUND_HELPER_BACKGROUND_HELPER_LINUX_H_

#include "base/memory/weak_ptr.h"
#include "hns/browser/hns_ads/background_helper/background_helper.h"
#include "chrome/browser/ui/browser_list_observer.h"  // IWYU pragma: keep

namespace hns_ads {

class BackgroundHelperLinux
    : public BackgroundHelper,
      public base::SupportsWeakPtr<BackgroundHelperLinux>,
      public BrowserListObserver {
 public:
  BackgroundHelperLinux(const BackgroundHelperLinux&) = delete;
  BackgroundHelperLinux& operator=(const BackgroundHelperLinux&) = delete;

  BackgroundHelperLinux(BackgroundHelperLinux&&) noexcept = delete;
  BackgroundHelperLinux& operator=(BackgroundHelperLinux&&) noexcept = delete;

  ~BackgroundHelperLinux() override;

 protected:
  friend class BackgroundHelperHolder;

  BackgroundHelperLinux();

 private:
  // BrowserListObserver overrides
  void OnBrowserSetLastActive(Browser* browser) override;
  void OnBrowserNoLongerActive(Browser* browser) override;

  // BackgroundHelper impl
  bool IsForeground() const override;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_BACKGROUND_HELPER_BACKGROUND_HELPER_LINUX_H_
