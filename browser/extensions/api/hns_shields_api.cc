/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/api/hns_shields_api.h"

#include <utility>

#include "hns/browser/hns_browser_process.h"
#include "hns/browser/ui/hns_pages.h"
#include "hns/common/extensions/api/hns_shields.h"
#include "hns/components/hns_shields/browser/ad_block_custom_filters_provider.h"
#include "hns/components/hns_shields/browser/ad_block_service.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/cookie_settings_factory.h"
#include "chrome/browser/ui/browser_finder.h"
#include "components/content_settings/core/browser/cookie_settings.h"

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction
HnsShieldsAddSiteCosmeticFilterFunction::Run() {
  absl::optional<hns_shields::AddSiteCosmeticFilter::Params> params =
      hns_shields::AddSiteCosmeticFilter::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  g_hns_browser_process->ad_block_service()
      ->custom_filters_provider()
      ->HideElementOnHost(params->css_selector, params->host);

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
HnsShieldsOpenFilterManagementPageFunction::Run() {
  Browser* browser = chrome::FindLastActive();
  if (browser) {
    hns::ShowHnsAdblock(browser);
  }

  return RespondNow(NoArguments());
}

}  // namespace api
}  // namespace extensions
