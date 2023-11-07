/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_ATTRIBUTES_STORAGE_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_ATTRIBUTES_STORAGE_H_

#include "hns/browser/profiles/hns_profile_avatar_downloader.h"

#define ProfileAvatarDownloader HnsProfileAvatarDownloader
#include "src/chrome/browser/profiles/profile_attributes_storage.h"  // IWYU pragma: export
#undef ProfileAvatarDownloader

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_ATTRIBUTES_STORAGE_H_
