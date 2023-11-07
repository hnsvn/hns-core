/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define HNS_PAGE_INFO_SHOULD_SHOW_PERMISSION          \
  if (!delegate_->HnsShouldShowPermission(info.type)) \
    return false;

#include "src/components/page_info/page_info.cc"
#undef HNS_PAGE_INFO_SHOULD_SHOW_PERMISSION
