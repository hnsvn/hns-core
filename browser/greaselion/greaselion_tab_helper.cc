/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/greaselion/greaselion_tab_helper.h"

#include <string>
#include <vector>

#include "base/functional/callback_helpers.h"
#include "base/strings/utf_string_conversions.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/browser/greaselion/greaselion_service_factory.h"
#include "hns/components/greaselion/browser/greaselion_download_service.h"
#include "hns/components/greaselion/browser/greaselion_service.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_user_data.h"

namespace greaselion {

GreaselionTabHelper::GreaselionTabHelper(content::WebContents* web_contents)
    : WebContentsObserver(web_contents),
      content::WebContentsUserData<GreaselionTabHelper>(*web_contents) {
  download_service_ = g_hns_browser_process->greaselion_download_service();
  download_service_->AddObserver(this);
}

GreaselionTabHelper::~GreaselionTabHelper() {
  download_service_->RemoveObserver(this);
}

void GreaselionTabHelper::OnRulesReady(
    GreaselionDownloadService* download_service) {
  auto* greaselion_service = GreaselionServiceFactory::GetForBrowserContext(
      web_contents()->GetBrowserContext());
  if (greaselion_service)
    greaselion_service->UpdateInstalledExtensions();
}

WEB_CONTENTS_USER_DATA_KEY_IMPL(GreaselionTabHelper);

}  // namespace greaselion
