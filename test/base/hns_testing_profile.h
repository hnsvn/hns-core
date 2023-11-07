/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_TEST_BASE_HNS_TESTING_PROFILE_H_
#define HNS_TEST_BASE_HNS_TESTING_PROFILE_H_

#include "chrome/test/base/testing_profile.h"

class HnsTestingProfile : public TestingProfile {
 public:
  HnsTestingProfile();
  HnsTestingProfile(const base::FilePath& path, Delegate* delegate);
  ~HnsTestingProfile() override = default;
};

#endif  // HNS_TEST_BASE_HNS_TESTING_PROFILE_H_
