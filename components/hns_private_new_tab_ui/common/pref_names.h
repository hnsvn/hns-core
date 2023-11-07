/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_PRIVATE_NEW_TAB_UI_COMMON_PREF_NAMES_H_
#define HNS_COMPONENTS_HNS_PRIVATE_NEW_TAB_UI_COMMON_PREF_NAMES_H_

class PrefRegistrySimple;

namespace hns_private_new_tab::prefs {

constexpr char kHnsPrivateWindowDisclaimerDismissed[] =
    "hns.hns_private_new_tab.private_window_disclaimer_dismissed";
constexpr char kHnsTorWindowDisclaimerDismissed[] =
    "hns.hns_private_new_tab.tor_window_disclaimer_dismissed";

void RegisterProfilePrefs(PrefRegistrySimple* registry);

}  // namespace hns_private_new_tab::prefs

#endif  // HNS_COMPONENTS_HNS_PRIVATE_NEW_TAB_UI_COMMON_PREF_NAMES_H_
