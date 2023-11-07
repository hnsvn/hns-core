/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "hns/ios/browser/favicon/hns_ios_favicon_loader_factory.h"
#import "hns/ios/browser/favicon/hns_ios_favicon_loader.h"

#include "base/no_destructor.h"
#include "components/keyed_service/core/service_access_type.h"
#include "components/keyed_service/ios/browser_state_dependency_manager.h"
#include "ios/chrome/browser/favicon/favicon_service_factory.h"
#import "ios/chrome/browser/favicon/ios_chrome_large_icon_service_factory.h"
#include "ios/chrome/browser/shared/model/browser_state/browser_state_otr_helper.h"
#include "ios/chrome/browser/shared/model/browser_state/chrome_browser_state.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

namespace {

std::unique_ptr<KeyedService> BuildFaviconLoader(web::BrowserState* context) {
  ChromeBrowserState* browser_state =
      ChromeBrowserState::FromBrowserState(context);
  return std::make_unique<hns_favicon::HnsFaviconLoader>(
      ios::FaviconServiceFactory::GetForBrowserState(
          browser_state, ServiceAccessType::EXPLICIT_ACCESS));
}

}  // namespace

namespace hns_favicon {
HnsFaviconLoader* HnsIOSFaviconLoaderFactory::GetForBrowserState(
    ChromeBrowserState* browser_state) {
  return static_cast<HnsFaviconLoader*>(
      GetInstance()->GetServiceForBrowserState(browser_state, true));
}

HnsFaviconLoader* HnsIOSFaviconLoaderFactory::GetForBrowserStateIfExists(
    ChromeBrowserState* browser_state) {
  return static_cast<HnsFaviconLoader*>(
      GetInstance()->GetServiceForBrowserState(browser_state, false));
}

HnsIOSFaviconLoaderFactory* HnsIOSFaviconLoaderFactory::GetInstance() {
  static base::NoDestructor<HnsIOSFaviconLoaderFactory> instance;
  return instance.get();
}

// static
BrowserStateKeyedServiceFactory::TestingFactory
HnsIOSFaviconLoaderFactory::GetDefaultFactory() {
  return base::BindRepeating(&BuildFaviconLoader);
}

HnsIOSFaviconLoaderFactory::HnsIOSFaviconLoaderFactory()
    : BrowserStateKeyedServiceFactory(
          "HnsFaviconLoader",
          BrowserStateDependencyManager::GetInstance()) {
  DependsOn(ios::FaviconServiceFactory::GetInstance());
  DependsOn(IOSChromeLargeIconServiceFactory::GetInstance());
}

HnsIOSFaviconLoaderFactory::~HnsIOSFaviconLoaderFactory() {}

std::unique_ptr<KeyedService>
HnsIOSFaviconLoaderFactory::BuildServiceInstanceFor(
    web::BrowserState* context) const {
  return BuildFaviconLoader(context);
}

web::BrowserState* HnsIOSFaviconLoaderFactory::GetBrowserStateToUse(
    web::BrowserState* context) const {
  return GetBrowserStateRedirectedInIncognito(context);
}

bool HnsIOSFaviconLoaderFactory::ServiceIsNULLWhileTesting() const {
  return true;
}
}  // namespace hns_favicon
