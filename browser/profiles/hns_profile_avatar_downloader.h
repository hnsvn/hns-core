/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_PROFILES_HNS_PROFILE_AVATAR_DOWNLOADER_H_
#define HNS_BROWSER_PROFILES_HNS_PROFILE_AVATAR_DOWNLOADER_H_

#include "chrome/browser/profiles/profile_avatar_downloader.h"

class HnsProfileAvatarDownloader : public ProfileAvatarDownloader {
 public:
  HnsProfileAvatarDownloader(size_t icon_index,
                               FetchCompleteCallback callback);
  ~HnsProfileAvatarDownloader() override;

  void Start();

 private:
  FetchCompleteCallback callback_;
};

#endif  // HNS_BROWSER_PROFILES_HNS_PROFILE_AVATAR_DOWNLOADER_H_
