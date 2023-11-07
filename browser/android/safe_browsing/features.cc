/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/android/safe_browsing/features.h"

#include "base/feature_list.h"

namespace safe_browsing {
namespace features {

BASE_FEATURE(kHnsAndroidSafeBrowsing,
             "HnsAndroidSafeBrowsing",
             base::FEATURE_ENABLED_BY_DEFAULT);

}  // namespace features
}  // namespace safe_browsing
