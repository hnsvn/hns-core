/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_FAVICON_HNS_IOS_FAVICON_LOADER_FACTORY_H_
#define HNS_IOS_BROWSER_FAVICON_HNS_IOS_FAVICON_LOADER_FACTORY_H_

#include <memory>

#include "base/no_destructor.h"
#include "components/keyed_service/ios/browser_state_keyed_service_factory.h"

class ChromeBrowserState;

namespace hns_favicon {
class HnsFaviconLoader;

// Singleton that owns all FaviconLoaders and associates them with
// ChromeBrowserState.
class HnsIOSFaviconLoaderFactory : public BrowserStateKeyedServiceFactory {
 public:
  static HnsFaviconLoader* GetForBrowserState(
      ChromeBrowserState* browser_state);
  static HnsFaviconLoader* GetForBrowserStateIfExists(
      ChromeBrowserState* browser_state);
  static HnsIOSFaviconLoaderFactory* GetInstance();
  static TestingFactory GetDefaultFactory();

  HnsIOSFaviconLoaderFactory(const HnsIOSFaviconLoaderFactory&) = delete;
  HnsIOSFaviconLoaderFactory& operator=(const HnsIOSFaviconLoaderFactory&) =
      delete;

 private:
  friend class base::NoDestructor<HnsIOSFaviconLoaderFactory>;

  HnsIOSFaviconLoaderFactory();
  ~HnsIOSFaviconLoaderFactory() override;

  // BrowserStateKeyedServiceFactory implementation.
  std::unique_ptr<KeyedService> BuildServiceInstanceFor(
      web::BrowserState* context) const override;
  web::BrowserState* GetBrowserStateToUse(
      web::BrowserState* context) const override;
  bool ServiceIsNULLWhileTesting() const override;
};
}  // namespace hns_favicon

#endif  // HNS_IOS_BROWSER_FAVICON_HNS_IOS_FAVICON_LOADER_FACTORY_H_
