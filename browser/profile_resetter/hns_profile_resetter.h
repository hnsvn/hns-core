/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_PROFILE_RESETTER_HNS_PROFILE_RESETTER_H_
#define HNS_BROWSER_PROFILE_RESETTER_HNS_PROFILE_RESETTER_H_

#include "chrome/browser/profile_resetter/profile_resetter.h"

// Reset hns specific prefs.
class HnsProfileResetter : public ProfileResetter {
 public:
  using ProfileResetter::ProfileResetter;
  HnsProfileResetter(const HnsProfileResetter&) = delete;
  HnsProfileResetter& operator=(const HnsProfileResetter&) = delete;
  ~HnsProfileResetter() override;

  // ProfileResetter overrides:
  void ResetDefaultSearchEngine() override;
};

#endif  // HNS_BROWSER_PROFILE_RESETTER_HNS_PROFILE_RESETTER_H_
