/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/themes/hns_theme_service.h"
// Below files are included in advance to prevent overriding
// GetBrowserContextToUse.
#include "chrome/browser/extensions/extension_system_factory.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "extensions/browser/extension_prefs_factory.h"
#include "extensions/browser/extension_registry_factory.h"

#if !BUILDFLAG(IS_LINUX)
#define HNS_THEMESERVICEFACTORY_BUILDSERVICEINSTANCEFOR \
  using ThemeService = HnsThemeService;
#else
// On Linux ThemeServiceAuraLinux derives from HnsThemeService instead.
#define HNS_THEMESERVICEFACTORY_BUILDSERVICEINSTANCEFOR
#endif

#include "src/chrome/browser/themes/theme_service_factory.cc"
#undef HNS_THEMESERVICEFACTORY_BUILDSERVICEINSTANCEFOR

content::BrowserContext* ThemeServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  if (context->IsTor())
    return context;
  return ProfileKeyedServiceFactory::GetBrowserContextToUse(context);
}
