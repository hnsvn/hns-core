/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_INFOBARS_CORE_INFOBAR_DELEGATE_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_INFOBARS_CORE_INFOBAR_DELEGATE_H_

// Add corresponding value into
// hns/android/java/org/chromium/chrome/browser/infobar/HnsInfoBarIdentifier.java
// When there will be too many items, redo java_cpp_enum.py to generate it
// automatically

#define HNS_INFOBAR_DELEGATE_IDENTIFIERS                                   \
  HNS_CONFIRM_P3A_INFOBAR_DELEGATE = 500,                                  \
  WAYBACK_MACHINE_INFOBAR_DELEGATE = 502,                                    \
  SYNC_V2_MIGRATE_INFOBAR_DELEGATE = 503,                                    \
  ANDROID_SYSTEM_SYNC_DISABLED_INFOBAR = 504, SYNC_CANNOT_RUN_INFOBAR = 505, \
  WEB_DISCOVERY_INFOBAR_DELEGATE = 506,                                      \
  HNS_SYNC_ACCOUNT_DELETED_INFOBAR = 507,                                  \
  HNS_REQUEST_OTR_INFOBAR_DELEGATE = 508, HNS_IPFS_INFOBAR_DELEGATE = 509,

#include "src/components/infobars/core/infobar_delegate.h"  // IWYU pragma: export

#undef HNS_INFOBAR_DELEGATE_IDENTIFIERS

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_INFOBARS_CORE_INFOBAR_DELEGATE_H_
