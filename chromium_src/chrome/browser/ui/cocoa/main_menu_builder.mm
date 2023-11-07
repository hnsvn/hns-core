/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/app/hns_command_ids.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/grit/generated_resources.h"

namespace {
constexpr int kPasteMacResourceId = IDS_PASTE_MAC;
constexpr int kMuteSiteResourceId = IDS_MUTE_SITE_MAC;
}  // namespace

#define HNS_BUILD_FILE_MENU                       \
  Item(IDS_NEW_OFFTHERECORD_WINDOW_TOR)             \
      .command_id(IDC_NEW_OFFTHERECORD_WINDOW_TOR),

#define HNS_BUILD_HELP_MENU                         \
  Item(IDS_REPORT_BROKEN_SITE_MAC)                    \
      .command_id(IDC_SHOW_HNS_WEBCOMPAT_REPORTER),

#undef IDS_PASTE_MAC
#define IDS_PASTE_MAC IDS_COPY_CLEAN_LINK) \
                    .command_id(IDC_COPY_CLEAN_LINK) \
                    .target(app_delegate), \
        Item(kPasteMacResourceId

#undef IDS_MUTE_SITE_MAC
#define IDS_MUTE_SITE_MAC                          \
IDS_MUTE_TAB_MAC).command_id(IDC_TOGGLE_TAB_MUTE), \
              Item(kMuteSiteResourceId

#include "src/chrome/browser/ui/cocoa/main_menu_builder.mm"
#undef IDS_MUTE_SITE_MAC
#undef HNS_WEBCOMPAT_REPORTER_MENU_ENTRY
#undef IDS_PASTE_MAC
#define IDS_PASTE_MAC kPasteMacResourceId
#define IDS_MUTE_SITE_MAC kMuteSiteResourceId
