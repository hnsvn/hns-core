/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_vpn/win/hns_vpn_wireguard_service/resources/resource_loader.h"

#include <memory>

#include "base/files/file_util.h"
#include "base/path_service.h"
#include "hns/components/constants/hns_paths.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace hns_vpn {

TEST(ResourceLoaderTest, FindPakPath) {
  base::FilePath test_data_dir;
  base::PathService::Get(hns::DIR_TEST_DATA, &test_data_dir);
  base::FilePath wireguard = test_data_dir.Append(L"wireguard");
  // Looking to upper directory.
  EXPECT_EQ(
      FindPakFilePath(wireguard.Append(L"HnsVpnWireguardService"), "en-US"),
      base::FilePath(wireguard.Append(L"Locales").Append(L"en-US.pak")));
  // Looking to current directory.
  EXPECT_EQ(FindPakFilePath(wireguard, "en-US"),
            base::FilePath(wireguard.Append(L"Locales").Append(L"en-US.pak")));
  // Fallback to english locale.
  EXPECT_EQ(FindPakFilePath(wireguard, "de-DE"),
            base::FilePath(wireguard.Append(L"Locales").Append(L"en-US.pak")));
}

}  // namespace hns_vpn
