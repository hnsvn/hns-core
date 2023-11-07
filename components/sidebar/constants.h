/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_SIDEBAR_CONSTANTS_H_
#define HNS_COMPONENTS_SIDEBAR_CONSTANTS_H_

namespace sidebar {

constexpr char kSidebarItemURLKey[] = "url";
constexpr char kSidebarItemTypeKey[] = "type";
constexpr char kSidebarItemBuiltInItemTypeKey[] = "built_in_item_type";
constexpr char kSidebarItemTitleKey[] = "title";
constexpr char kSidebarItemOpenInPanelKey[] = "open_in_panel";
constexpr int kDefaultSidePanelWidth = 320;

// list is provided from chrome layer.
constexpr char kHnsTalkURL[] = "https://talk.hns.com/widget";
constexpr char kHnsTalkHost[] = "talk.hns.com";

}  // namespace sidebar

#endif  // HNS_COMPONENTS_SIDEBAR_CONSTANTS_H_
