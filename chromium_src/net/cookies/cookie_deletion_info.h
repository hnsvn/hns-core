/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_NET_COOKIES_COOKIE_DELETION_INFO_H_
#define HNS_CHROMIUM_SRC_NET_COOKIES_COOKIE_DELETION_INFO_H_

#define HNS_COOKIE_DELETION_INFO_H \
  absl::optional<std::string> ephemeral_storage_domain;

#include "src/net/cookies/cookie_deletion_info.h"  // IWYU pragma: export

#undef HNS_COOKIE_DELETION_INFO_H

#endif  // HNS_CHROMIUM_SRC_NET_COOKIES_COOKIE_DELETION_INFO_H_
