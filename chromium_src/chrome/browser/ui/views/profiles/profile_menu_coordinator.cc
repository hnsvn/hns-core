/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/profiles/hns_incognito_menu_view.h"
#include "hns/browser/ui/views/profiles/hns_profile_menu_view.h"

#include "chrome/browser/ui/views/profiles/incognito_menu_view.h"
#include "chrome/browser/ui/views/profiles/profile_menu_view.h"

#define IncognitoMenuView HnsIncognitoMenuView
#define ProfileMenuView HnsProfileMenuView
#include "src/chrome/browser/ui/views/profiles/profile_menu_coordinator.cc"
#undef IncognitoMenuView
#undef ProfileMenuView
