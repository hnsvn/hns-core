/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_TABS_HNS_TAB_PREFS_H_
#define HNS_BROWSER_UI_TABS_HNS_TAB_PREFS_H_

class PrefRegistrySimple;
class PrefService;

namespace hns_tabs {

enum TabHoverMode { TOOLTIP = 0, CARD = 1, CARD_WITH_PREVIEW = 2 };

extern const char kTabHoverMode[];

extern const char kVerticalTabsEnabled[];
extern const char kVerticalTabsCollapsed[];
extern const char kVerticalTabsShowTitleOnWindow[];
extern const char kVerticalTabsFloatingEnabled[];
extern const char kVerticalTabsExpandedWidth[];

void RegisterHnsProfilePrefs(PrefRegistrySimple* registry);

bool AreTooltipsEnabled(PrefService* prefs);
bool AreCardPreviewsEnabled(PrefService* prefs);
}  // namespace hns_tabs

#endif  // HNS_BROWSER_UI_TABS_HNS_TAB_PREFS_H_
