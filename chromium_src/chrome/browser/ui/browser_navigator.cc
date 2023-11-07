/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/constants/webui_url_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_navigator_params.h"
#include "chrome/common/webui_url_constants.h"
#include "url/gurl.h"

namespace {

void UpdateHnsScheme(NavigateParams* params) {
  if (params->url.SchemeIs(content::kHnsUIScheme)) {
    GURL::Replacements replacements;
    replacements.SetSchemeStr(content::kChromeUIScheme);
    params->url = params->url.ReplaceComponents(replacements);
  }
}

bool IsHostAllowedInIncognitoHnsImpl(const base::StringPiece& host) {
  if (host == kWalletPageHost || host == kWalletPanelHost ||
      host == kRewardsPageHost || host == chrome::kChromeUISyncInternalsHost ||
      host == chrome::kChromeUISyncHost || host == kAdblockHost ||
      host == kWelcomeHost) {
    return false;
  }

  return true;
}

}  // namespace

#define HNS_ADJUST_NAVIGATE_PARAMS_FOR_URL UpdateHnsScheme(params);
#include "src/chrome/browser/ui/browser_navigator.cc"
#undef HNS_ADJUST_NAVIGATE_PARAMS_FOR_URL