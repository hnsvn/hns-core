/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/tabs/hns_new_tab_button.h"
#include "hns/browser/ui/views/tabs/hns_tab_search_button.h"

#define NewTabButton HnsNewTabButton
#define TabSearchButton HnsTabSearchButton
#include "src/chrome/browser/ui/views/frame/tab_strip_region_view.cc"
#undef TabSearchButton
#undef NewTabButton
