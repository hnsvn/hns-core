/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_SIDEBAR_PREF_NAMES_H_
#define HNS_COMPONENTS_SIDEBAR_PREF_NAMES_H_

namespace sidebar {

constexpr char kSidebarItems[] = "hns.sidebar.sidebar_items";
constexpr char kSidebarHiddenBuiltInItems[] =
    "hns.sidebar.hidden_built_in_items";
constexpr char kSidebarShowOption[] = "hns.sidebar.sidebar_show_option";
constexpr char kSidebarItemAddedFeedbackBubbleShowCount[] =
    "hns.sidebar.item_added_feedback_bubble_shown_count";
constexpr char kSidePanelWidth[] = "hns.sidebar.side_panel_width";

// Indicates that sidebar alignment was changed by the browser itself, not by
// users.
constexpr char kSidebarAlignmentChangedTemporarily[] =
    "hns.sidebar.sidebar_alignment_changed_for_vertical_tabs";

}  // namespace sidebar

#endif  // HNS_COMPONENTS_SIDEBAR_PREF_NAMES_H_
