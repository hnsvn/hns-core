/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/api/hns_wallet/token_registry_utils.h"

#include "base/files/file_path.h"
#include "base/path_service.h"
#include "base/strings/sys_string_conversions.h"
#include "base/version.h"
#include "hns/components/hns_wallet/browser/hns_wallet_constants.h"
#include "hns/components/hns_wallet/browser/wallet_data_files_installer.h"
#include "ios/chrome/browser/shared/model/paths/paths.h"

@implementation HnsWalletTokenRegistryUtils

+ (nullable NSURL*)tokenLogoBaseURL {
  base::FilePath path;
  absl::optional<base::Version> version =
      hns_wallet::GetLastInstalledWalletVersion();
  if (base::PathService::Get(ios::DIR_USER_DATA, &path) && version) {
    path = path.AppendASCII(hns_wallet::kWalletBaseDirectory);
    path = path.AppendASCII(version->GetString());
    path = path.AppendASCII("images");
    return [NSURL fileURLWithPath:base::SysUTF8ToNSString(path.value())];
  }
  return nil;
}

@end
