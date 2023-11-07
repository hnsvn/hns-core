/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/navigation_bar_data_provider.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/webui_util.h"
#include "content/public/browser/web_ui_data_source.h"

namespace {

// Called from the original password_manager_ui.cc's
// CreateAndAddPasswordsUIHTMLSource via a patch.
void HnsAddPasswordManagerResources(content::WebUIDataSource* source,
                                      Profile* profile) {
  NavigationBarDataProvider::Initialize(source, profile);
}

}  // namespace

#define SetupChromeRefresh2023(SOURCE) \
  SetupChromeRefresh2023(SOURCE);      \
  HnsAddPasswordManagerResources(source, profile);

#include "src/chrome/browser/ui/webui/password_manager/password_manager_ui.cc"
#undef SetupChromeRefresh2023
