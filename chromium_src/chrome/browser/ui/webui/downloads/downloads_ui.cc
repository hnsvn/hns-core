/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/navigation_bar_data_provider.h"
#include "chrome/browser/profiles/profile.h"

namespace {

void HnsCustomizeDownloadsDataSource(content::WebUIDataSource* source,
                                       Profile* profile) {
  NavigationBarDataProvider::Initialize(source, profile);
}

}  // namespace

#define HNS_CREATE_DOWNLOADS_UI_HTML_SOURCE \
  HnsCustomizeDownloadsDataSource(source, profile);

#include "src/chrome/browser/ui/webui/downloads/downloads_ui.cc"
#undef HNS_CREATE_DOWNLOADS_UI_HTML_SOURCE
