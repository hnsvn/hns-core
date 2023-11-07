/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_SERVICES_BAT_ADS_SERVICE_FACTORY_IMPL_H_
#define HNS_BROWSER_HNS_ADS_SERVICES_BAT_ADS_SERVICE_FACTORY_IMPL_H_

#include "hns/components/hns_ads/browser/bat_ads_service_factory.h"
#include "hns/components/services/bat_ads/public/interfaces/bat_ads.mojom.h"
#include "mojo/public/cpp/bindings/remote.h"

namespace hns_ads {

class BatAdsServiceFactoryImpl : public BatAdsServiceFactory {
 public:
  BatAdsServiceFactoryImpl();

  BatAdsServiceFactoryImpl(const BatAdsServiceFactoryImpl&) = delete;
  BatAdsServiceFactoryImpl& operator=(const BatAdsServiceFactoryImpl&) = delete;

  BatAdsServiceFactoryImpl(BatAdsServiceFactoryImpl&&) noexcept = delete;
  BatAdsServiceFactoryImpl& operator=(BatAdsServiceFactoryImpl&&) noexcept =
      delete;

  ~BatAdsServiceFactoryImpl() override;

  // BatAdsServiceFactory:
  mojo::Remote<bat_ads::mojom::BatAdsService> Launch() const override;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_SERVICES_BAT_ADS_SERVICE_FACTORY_IMPL_H_
