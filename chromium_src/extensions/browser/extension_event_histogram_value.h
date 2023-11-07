/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_EXTENSIONS_BROWSER_EXTENSION_EVENT_HISTOGRAM_VALUE_H_
#define HNS_CHROMIUM_SRC_EXTENSIONS_BROWSER_EXTENSION_EVENT_HISTOGRAM_VALUE_H_

// clang-format off
#define ENUM_BOUNDARY                         \
  HNS_START = 600,                          \
  HNS_AD_BLOCKED,                           \
  HNS_WALLET_CREATED,                       \
  HNS_ON_WALLET_PROPERTIES,                 \
  HNS_ON_PUBLISHER_DATA,                    \
  HNS_ON_CURRENT_REPORT,                    \
  HNS_ON_HNS_THEME_TYPE_CHANGED,          \
  HNS_REWARDS_NOTIFICATION_ADDED,           \
  HNS_REWARDS_NOTIFICATION_DELETED,         \
  HNS_REWARDS_ALL_NOTIFICATIONS_DELETED,    \
  HNS_REWARDS_GET_NOTIFICATION,             \
  HNS_REWARDS_GET_ALL_NOTIFICATIONS,        \
  HNS_WALLET_FAILED,                        \
  ENUM_BOUNDARY
// clang-format on

#include "src/extensions/browser/extension_event_histogram_value.h"  // IWYU pragma: export

#undef ENUM_BOUNDARY

#endif  // HNS_CHROMIUM_SRC_EXTENSIONS_BROWSER_EXTENSION_EVENT_HISTOGRAM_VALUE_H_
