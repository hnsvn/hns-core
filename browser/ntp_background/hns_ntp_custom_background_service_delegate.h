// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_NTP_BACKGROUND_HNS_NTP_CUSTOM_BACKGROUND_SERVICE_DELEGATE_H_
#define HNS_BROWSER_NTP_BACKGROUND_HNS_NTP_CUSTOM_BACKGROUND_SERVICE_DELEGATE_H_

#include <memory>
#include <string>

#include "base/functional/callback.h"
#include "base/functional/callback_helpers.h"
#include "base/gtest_prod_util.h"
#include "base/memory/raw_ptr.h"
#include "hns/components/ntp_background_images/browser/hns_ntp_custom_background_service.h"

class CustomBackgroundFileManager;
class Profile;

class HnsNTPCustomBackgroundServiceDelegate
    : public ntp_background_images::HnsNTPCustomBackgroundService::Delegate {
 public:
  explicit HnsNTPCustomBackgroundServiceDelegate(Profile* profile);
  ~HnsNTPCustomBackgroundServiceDelegate() override;
  HnsNTPCustomBackgroundServiceDelegate(
      const HnsNTPCustomBackgroundServiceDelegate&) = delete;
  HnsNTPCustomBackgroundServiceDelegate& operator=(
      const HnsNTPCustomBackgroundServiceDelegate&) = delete;

 private:
  FRIEND_TEST_ALL_PREFIXES(HnsNTPCustomBackgroundServiceDelegateUnitTest,
                           MigrationSuccess);
  FRIEND_TEST_ALL_PREFIXES(HnsNTPCustomBackgroundServiceDelegateUnitTest,
                           MigrationFail);

  bool ShouldMigrateCustomImagePref() const;
  void MigrateCustomImage(
      base::OnceCallback<void(bool)> callback = base::DoNothing());

  // HnsNTPCustomBackgroundService::Delegate overrides:
  bool IsCustomImageBackgroundEnabled() const override;
  base::FilePath GetCustomBackgroundImageLocalFilePath(
      const GURL& url) const override;
  GURL GetCustomBackgroundImageURL() const override;
  bool IsColorBackgroundEnabled() const override;
  std::string GetColor() const override;
  bool ShouldUseRandomValue() const override;
  bool HasPreferredHnsBackground() const override;
  base::Value::Dict GetPreferredHnsBackground() const override;

  raw_ptr<Profile> profile_ = nullptr;

  std::unique_ptr<CustomBackgroundFileManager> file_manager_;
};

#endif  // HNS_BROWSER_NTP_BACKGROUND_HNS_NTP_CUSTOM_BACKGROUND_SERVICE_DELEGATE_H_
