/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_BROWSER_ADS_SERVICE_OBSERVER_H_
#define HNS_COMPONENTS_HNS_ADS_BROWSER_ADS_SERVICE_OBSERVER_H_

#include "base/observer_list_types.h"

namespace hns_ads {

class AdsServiceObserver : public base::CheckedObserver {
 public:
  // Invoked when ad rewards have changed.
  virtual void OnAdRewardsDidChange() {}

  // Invoked when a browser upgrade is required to serve ads.
  virtual void OnNeedsBrowserUpgradeToServeAds() {}
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_BROWSER_ADS_SERVICE_OBSERVER_H_
