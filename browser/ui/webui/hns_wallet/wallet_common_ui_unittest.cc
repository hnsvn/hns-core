/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/hns_wallet/wallet_common_ui.h"
#include "hns/components/constants/webui_url_constants.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"
#include "url/origin.h"

namespace hns_wallet {

TEST(HnsWalletCommonUIUnitTest, IsHnsWalletOrigin) {
  ASSERT_TRUE(
      IsHnsWalletOrigin(url::Origin::Create(GURL(kHnsUIWalletPanelURL))));
  ASSERT_TRUE(
      IsHnsWalletOrigin(url::Origin::Create(GURL(kHnsUIWalletPageURL))));
  ASSERT_FALSE(IsHnsWalletOrigin(url::Origin::Create(GURL("https://a.com"))));
  ASSERT_FALSE(IsHnsWalletOrigin(url::Origin::Create(GURL())));
}

}  // namespace hns_wallet
