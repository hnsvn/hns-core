/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_PROFILES_HNS_PROFILE_MANAGER_H_
#define HNS_BROWSER_PROFILES_HNS_PROFILE_MANAGER_H_

#include <string>

#include "chrome/browser/profiles/profile_manager.h"

class HnsProfileManager : public ProfileManager {
 public:
  explicit HnsProfileManager(const base::FilePath& user_data_dir);
  HnsProfileManager(const HnsProfileManager&) = delete;
  HnsProfileManager& operator=(const HnsProfileManager&) = delete;

  void InitProfileUserPrefs(Profile* profile) override;
  void SetNonPersonalProfilePrefs(Profile* profile) override;
  bool IsAllowedProfilePath(const base::FilePath& path) const override;
  bool LoadProfileByPath(const base::FilePath& profile_path,
                         bool incognito,
                         ProfileLoadedCallback callback) override;

 protected:
  void DoFinalInitForServices(Profile* profile,
                              bool go_off_the_record) override;

 private:
  void MigrateProfileNames();
};

class HnsProfileManagerWithoutInit : public HnsProfileManager {
 public:
  HnsProfileManagerWithoutInit(const HnsProfileManagerWithoutInit&) =
      delete;
  HnsProfileManagerWithoutInit& operator=(
      const HnsProfileManagerWithoutInit&) = delete;
  explicit HnsProfileManagerWithoutInit(const base::FilePath& user_data_dir);
};

#endif  // HNS_BROWSER_PROFILES_HNS_PROFILE_MANAGER_H_
