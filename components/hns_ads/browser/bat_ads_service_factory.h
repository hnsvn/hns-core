/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_BROWSER_BAT_ADS_SERVICE_FACTORY_H_
#define HNS_COMPONENTS_HNS_ADS_BROWSER_BAT_ADS_SERVICE_FACTORY_H_

#include "hns/components/services/bat_ads/public/interfaces/bat_ads.mojom.h"
#include "mojo/public/cpp/bindings/remote.h"

namespace hns_ads {

class BatAdsServiceFactory {
 public:
  virtual ~BatAdsServiceFactory() = default;

  // Launches a new Bat Ads Service.
  virtual mojo::Remote<bat_ads::mojom::BatAdsService> Launch() const = 0;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_BROWSER_BAT_ADS_SERVICE_FACTORY_H_
