/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define HandleChromeAboutAndChromeSyncRewrite \
  HandleChromeAboutAndChromeSyncRewrite_ChromiumImpl
#include "src/chrome/browser/browser_about_handler.cc"
#undef HandleChromeAboutAndChromeSyncRewrite

#include "hns/components/constants/url_constants.h"
#include "hns/components/constants/webui_url_constants.h"

bool HandleChromeAboutAndChromeSyncRewrite(
    GURL* url,
    content::BrowserContext* browser_context) {
  if (url->SchemeIs(kHnsUIScheme)) {
    GURL::Replacements replacements;
    replacements.SetSchemeStr(content::kChromeUIScheme);
    *url = url->ReplaceComponents(replacements);
  }

  bool result =
      HandleChromeAboutAndChromeSyncRewrite_ChromiumImpl(url, browser_context);

  return result;
}
