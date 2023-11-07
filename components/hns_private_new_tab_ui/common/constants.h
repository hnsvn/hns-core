/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_PRIVATE_NEW_TAB_UI_COMMON_CONSTANTS_H_
#define HNS_COMPONENTS_HNS_PRIVATE_NEW_TAB_UI_COMMON_CONSTANTS_H_

#include "components/grit/hns_components_strings.h"
#include "ui/base/webui/web_ui_util.h"

namespace hns_private_new_tab {

constexpr webui::LocalizedString kLocalizedStrings[] = {
    {"headerTitle", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW},
    {"headerText", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW_DESC},
    {"headerText1", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW_DESC1},
    {"headerText2", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW_DESC2},
    {"headerButton", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW_BUTTON},
    {"headerTorTitle", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW_TOR},
    {"headerTorText", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW_TOR_DESC},
    {"torStatus", IDS_HNS_PRIVATE_NEW_TAB_TOR_STATUS},
    {"torStatusConnected",
     IDS_HNS_PRIVATE_NEW_TAB_TOR_STATUS_CONNECTED},  // NOLINT
    {"torStatusDisconnected",
     IDS_HNS_PRIVATE_NEW_TAB_TOR_STATUS_DISCONNECTED},  // NOLINT
    {"torStatusInitializing",
     IDS_HNS_PRIVATE_NEW_TAB_TOR_STATUS_INITIALIZING},  // NOLINT
    {"torStatusConnectionSlow",
     IDS_HNS_PRIVATE_NEW_TAB_TOR_STATUS_CONNECTION_SLOW},
    {"torStatusConnectionSlowDesc",
     IDS_HNS_PRIVATE_NEW_TAB_TOR_STATUS_CONNECTION_SLOW_DESC},
    {"torStatusConnectionFailed",
     IDS_HNS_PRIVATE_NEW_TAB_TOR_STATUS_CONNECTION_FAILED},
    {"torHelpConnecting", IDS_HNS_PRIVATE_NEW_TAB_TOR_HELP_CONNECTING},
    {"torHelpDisconnectedReenable",
     IDS_HNS_PRIVATE_NEW_TAB_TOR_HELP_DISCONNECTED_REENABLE},  // NOLINT
    {"torHelpDisconnectedBridges",
     IDS_HNS_PRIVATE_NEW_TAB_TOR_HELP_DISCONNECTED_BRIDGES},  // NOLINT
    {"torHelpContactSupport",
     IDS_HNS_PRIVATE_NEW_TAB_TOR_HELP_CONTACT_SUPPORT},  // NOLINT
    {"searchPlaceholderLabel",
     IDS_HNS_PRIVATE_NEW_TAB_SEARCH_PLACEHOLDER},  // NOLINT
};

}  // namespace hns_private_new_tab

#endif  // HNS_COMPONENTS_HNS_PRIVATE_NEW_TAB_UI_COMMON_CONSTANTS_H_
