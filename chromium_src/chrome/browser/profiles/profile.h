/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_H_

#define PrimaryID                 \
  PrimaryID();                    \
  friend class TorProfileManager; \
  static const OTRProfileID TorID
#define HasPrimaryOTRProfile IsTor() const override; bool HasPrimaryOTRProfile
#define IsIncognitoProfile                 \
  IsIncognitoProfile_ChromiumImpl() const; \
  bool IsIncognitoProfile
#define IsPrimaryOTRProfile                 \
  IsPrimaryOTRProfile_ChromiumImpl() const; \
  bool IsPrimaryOTRProfile

#include "src/chrome/browser/profiles/profile.h"  // IWYU pragma: export

#undef IsPrimaryOTRProfile
#undef IsIncognitoProfile
#undef HasPrimaryOTRProfile
#undef PrimaryID

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_H_
