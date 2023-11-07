// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_IOS_BROWSER_URL_SANITIZER_URL_SANITIZER_SERVICE_FACTORY_PRIVATE_H_
#define HNS_IOS_BROWSER_URL_SANITIZER_URL_SANITIZER_SERVICE_FACTORY_PRIVATE_H_

#include <memory>

#include "hns/ios/browser/keyed_service/keyed_service_factory_wrapper.h"
#include "components/keyed_service/ios/browser_state_keyed_service_factory.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace web {
class BrowserState;
}  // namespace web

class KeyedService;
class ChromeBrowserState;

namespace hns {
class URLSanitizerService;

class URLSanitizerServiceFactory : public BrowserStateKeyedServiceFactory {
 public:
  static hns::URLSanitizerService* GetServiceForState(
      ChromeBrowserState* browser_state);

  static URLSanitizerServiceFactory* GetInstance();

 protected:
  bool ServiceIsCreatedWithBrowserState() const override;

 private:
  friend base::NoDestructor<URLSanitizerServiceFactory>;

  URLSanitizerServiceFactory();
  ~URLSanitizerServiceFactory() override;

  // BrowserStateKeyedServiceFactory implementation.
  std::unique_ptr<KeyedService> BuildServiceInstanceFor(
      web::BrowserState* context) const override;
  bool ServiceIsNULLWhileTesting() const override;
  web::BrowserState* GetBrowserStateToUse(
      web::BrowserState* context) const override;
};

}  // namespace hns

#endif  // HNS_IOS_BROWSER_URL_SANITIZER_URL_SANITIZER_SERVICE_FACTORY_PRIVATE_H_
