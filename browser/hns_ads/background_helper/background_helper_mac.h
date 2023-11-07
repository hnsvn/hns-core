/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_BACKGROUND_HELPER_BACKGROUND_HELPER_MAC_H_
#define HNS_BROWSER_HNS_ADS_BACKGROUND_HELPER_BACKGROUND_HELPER_MAC_H_

#include <memory>

#include "hns/browser/hns_ads/background_helper/background_helper.h"

namespace hns_ads {

class BackgroundHelperMac : public BackgroundHelper {
 public:
  BackgroundHelperMac(const BackgroundHelperMac&) = delete;
  BackgroundHelperMac& operator=(const BackgroundHelperMac&) = delete;

  BackgroundHelperMac(BackgroundHelperMac&&) noexcept = delete;
  BackgroundHelperMac& operator=(BackgroundHelperMac&&) noexcept = delete;

  ~BackgroundHelperMac() override;

 protected:
  friend class BackgroundHelperHolder;

  BackgroundHelperMac();

 private:
  // BackgroundHelper impl
  bool IsForeground() const override;

  class BackgroundHelperDelegate;
  std::unique_ptr<BackgroundHelperDelegate> delegate_;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_BACKGROUND_HELPER_BACKGROUND_HELPER_MAC_H_
