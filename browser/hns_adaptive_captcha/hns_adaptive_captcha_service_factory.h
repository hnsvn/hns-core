/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADAPTIVE_CAPTCHA_HNS_ADAPTIVE_CAPTCHA_SERVICE_FACTORY_H_
#define HNS_BROWSER_HNS_ADAPTIVE_CAPTCHA_HNS_ADAPTIVE_CAPTCHA_SERVICE_FACTORY_H_

#include "hns/components/hns_adaptive_captcha/hns_adaptive_captcha_delegate.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

class Profile;

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace hns_adaptive_captcha {

class HnsAdaptiveCaptchaService;

class HnsAdaptiveCaptchaServiceFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  static HnsAdaptiveCaptchaService* GetForProfile(Profile* profile);
  static HnsAdaptiveCaptchaServiceFactory* GetInstance();

 private:
  friend base::NoDestructor<HnsAdaptiveCaptchaServiceFactory>;

  HnsAdaptiveCaptchaServiceFactory();
  ~HnsAdaptiveCaptchaServiceFactory() override;

  HnsAdaptiveCaptchaServiceFactory(
      const HnsAdaptiveCaptchaServiceFactory&) = delete;
  HnsAdaptiveCaptchaServiceFactory& operator=(
      const HnsAdaptiveCaptchaServiceFactory&) = delete;

  // BrowserContextKeyedServiceFactory overrides:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
};

}  // namespace hns_adaptive_captcha

#endif  // HNS_BROWSER_HNS_ADAPTIVE_CAPTCHA_HNS_ADAPTIVE_CAPTCHA_SERVICE_FACTORY_H_
