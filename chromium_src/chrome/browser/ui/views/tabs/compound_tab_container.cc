/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/tabs/hns_tab_container.h"

#define TabContainerImpl HnsTabContainer
#include "src/chrome/browser/ui/views/tabs/compound_tab_container.cc"
#undef TabContainerImpl
