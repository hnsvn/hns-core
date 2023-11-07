/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_PROFILE_MENU_VIEW_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_PROFILE_MENU_VIEW_H_

#define HNS_PROFILE_MENU_VIEW_H    \
  friend class HnsProfileMenuView; \

#define OnExitProfileButtonClicked virtual OnExitProfileButtonClicked
#define BuildAutofillButtons virtual BuildAutofillButtons
#define BuildIdentity virtual BuildIdentity
#define BuildSyncInfo virtual BuildSyncInfo
#define BuildFeatureButtons virtual BuildFeatureButtons

#include "src/chrome/browser/ui/views/profiles/profile_menu_view.h"  // IWYU pragma: export
#undef BuildFeatureButtons
#undef BuildSyncInfo
#undef BuildIdentity
#undef BuildAutofillButtons
#undef OnExitProfileButtonClicked
#undef HNS_PROFILE_MENU_VIEW_H

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_PROFILE_MENU_VIEW_H_
