/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_ADS_SERVICE_FACTORY_H_
#define HNS_BROWSER_HNS_ADS_ADS_SERVICE_FACTORY_H_

#include <memory>

#include "hns/browser/hns_ads/tooltips/ads_tooltips_delegate_impl.h"
#include "hns/components/hns_ads/browser/ads_service.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

class Profile;

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace hns_ads {

// Singleton that owns all AdsService and associates them with Profiles.
class AdsServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  AdsServiceFactory(const AdsServiceFactory&) = delete;
  AdsServiceFactory& operator=(const AdsServiceFactory&) = delete;

  AdsServiceFactory(AdsServiceFactory&&) noexcept = delete;
  AdsServiceFactory& operator=(AdsServiceFactory&&) noexcept = delete;

  static AdsService* GetForProfile(Profile* profile);

  static AdsServiceFactory* GetInstance();

 private:
  friend base::NoDestructor<AdsServiceFactory>;

  AdsServiceFactory();

  ~AdsServiceFactory() override;

  std::unique_ptr<AdsTooltipsDelegateImpl> CreateAdsTooltipsDelegate(
      Profile* profile) const;

  // BrowserContextKeyedServiceFactory:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
  bool ServiceIsNULLWhileTesting() const override;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_ADS_SERVICE_FACTORY_H_
