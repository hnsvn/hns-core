/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/prefs/hns_pref_service_incognito_allowlist.h"

#include "base/no_destructor.h"
#include "hns/components/ai_chat/common/buildflags/buildflags.h"
#include "hns/components/constants/pref_names.h"
#include "build/build_config.h"
#include "chrome/common/pref_names.h"

#if !BUILDFLAG(IS_ANDROID)
#include "hns/browser/ui/tabs/hns_tab_prefs.h"
#endif

#if BUILDFLAG(ENABLE_AI_CHAT)
#include "hns/components/ai_chat/common/pref_names.h"
#endif  // BUILDFLAG(ENABLE_AI_CHAT)

#if defined(TOOLKIT_VIEWS)
#include "hns/components/sidebar/pref_names.h"
#endif

namespace hns {

const std::vector<const char*>& GetHnsPersistentPrefNames() {
  static base::NoDestructor<std::vector<const char*>> hns_allowlist({
    kHnsAutofillPrivateWindows,
#if !BUILDFLAG(IS_ANDROID)
        prefs::kSidePanelHorizontalAlignment, kTabMuteIndicatorNotClickable,
        hns_tabs::kVerticalTabsExpandedWidth,
        hns_tabs::kVerticalTabsEnabled, hns_tabs::kVerticalTabsCollapsed,
        hns_tabs::kVerticalTabsFloatingEnabled,
#endif
#if defined(TOOLKIT_VIEWS)
        sidebar::kSidePanelWidth,
#endif
#if BUILDFLAG(ENABLE_AI_CHAT)
        ai_chat::prefs::kHnsChatHasSeenDisclaimer,
        ai_chat::prefs::kHnsChatAutoGenerateQuestions,
#endif  // BUILDFLAG(ENABLE_AI_CHAT)
  });

  return *hns_allowlist;
}

}  // namespace hns
