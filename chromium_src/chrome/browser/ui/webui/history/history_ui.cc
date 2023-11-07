// Copyright (c) 2019 The Hns Authors
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/webui/navigation_bar_data_provider.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/buildflags.h"
#include "content/public/browser/web_ui_data_source.h"

namespace {

void HnsCustomizeHistoryDataSource(content::WebUIDataSource* source,
                                     Profile* profile) {
  NavigationBarDataProvider::Initialize(source, profile);
}

}  // namespace

#define HNS_CREATE_HISTORY_UI_HTML_SOURCE \
  HnsCustomizeHistoryDataSource(source, profile);

#include "src/chrome/browser/ui/webui/history/history_ui.cc"
#undef HNS_CREATE_HISTORY_UI_HTML_SOURCE
