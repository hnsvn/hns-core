/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_NTP_BACKGROUND_HNS_NTP_CUSTOM_BACKGROUND_SERVICE_FACTORY_H_
#define HNS_BROWSER_NTP_BACKGROUND_HNS_NTP_CUSTOM_BACKGROUND_SERVICE_FACTORY_H_

#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace ntp_background_images {
class HnsNTPCustomBackgroundService;
}

class HnsNTPCustomBackgroundServiceFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  static ntp_background_images::HnsNTPCustomBackgroundService* GetForContext(
      content::BrowserContext* context);
  static HnsNTPCustomBackgroundServiceFactory* GetInstance();

 private:
  friend base::NoDestructor<HnsNTPCustomBackgroundServiceFactory>;

  HnsNTPCustomBackgroundServiceFactory();
  ~HnsNTPCustomBackgroundServiceFactory() override;

  HnsNTPCustomBackgroundServiceFactory(
      const HnsNTPCustomBackgroundServiceFactory&) = delete;
  HnsNTPCustomBackgroundServiceFactory& operator=(
      const HnsNTPCustomBackgroundServiceFactory&) = delete;

  // BrowserContextKeyedServiceFactory overrides:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
};

#endif  // HNS_BROWSER_NTP_BACKGROUND_HNS_NTP_CUSTOM_BACKGROUND_SERVICE_FACTORY_H_
