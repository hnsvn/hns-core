/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/tabs/hns_tab_prefs.h"

#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"

namespace hns_tabs {

const char kTabHoverMode[] = "hns.tabs.hover_mode";

const char kVerticalTabsEnabled[] = "hns.tabs.vertical_tabs_enabled";
const char kVerticalTabsCollapsed[] = "hns.tabs.vertical_tabs_collapsed";
const char kVerticalTabsShowTitleOnWindow[] =
    "hns.tabs.vertical_tabs_show_title_on_window";
const char kVerticalTabsFloatingEnabled[] =
    "hns.tabs.vertical_tabs_floating_enabled";
const char kVerticalTabsExpandedWidth[] =
    "hns.tabs.vertical_tabs_expanded_width";

void RegisterHnsProfilePrefs(PrefRegistrySimple* registry) {
  registry->RegisterIntegerPref(kTabHoverMode, TabHoverMode::CARD);
  registry->RegisterBooleanPref(kVerticalTabsEnabled, false);
  registry->RegisterBooleanPref(kVerticalTabsCollapsed, false);
#if BUILDFLAG(IS_WIN)
  // On Windows, we show window title by default
  // https://github.com/hnsvn/hns-browser/issues/30027
  registry->RegisterBooleanPref(kVerticalTabsShowTitleOnWindow, true);
#else
  registry->RegisterBooleanPref(kVerticalTabsShowTitleOnWindow, false);
#endif
  registry->RegisterBooleanPref(kVerticalTabsFloatingEnabled, true);
  registry->RegisterIntegerPref(kVerticalTabsExpandedWidth, 250);
}

bool AreTooltipsEnabled(PrefService* prefs) {
  return prefs->GetInteger(kTabHoverMode) == TabHoverMode::TOOLTIP;
}

bool AreCardPreviewsEnabled(PrefService* prefs) {
  return prefs->GetInteger(kTabHoverMode) == TabHoverMode::CARD_WITH_PREVIEW;
}

}  // namespace hns_tabs
