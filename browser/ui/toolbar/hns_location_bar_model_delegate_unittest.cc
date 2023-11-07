/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/toolbar/hns_location_bar_model_delegate.h"

#include "base/strings/utf_string_conversions.h"
#include "extensions/buildflags/buildflags.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_constants.h"

using HnsLocationBarModelDelegateTest = testing::Test;

TEST_F(HnsLocationBarModelDelegateTest, ResolvesEthereumRemoteClient) {
  GURL url(ethereum_remote_client_base_url);
  std::u16string formatted_url = base::UTF8ToUTF16(url.spec());
  HnsLocationBarModelDelegate::FormattedStringFromURL(url, &formatted_url);
  ASSERT_STREQ(base::UTF16ToASCII(formatted_url).c_str(), "hns://wallet");
}
TEST_F(HnsLocationBarModelDelegateTest,
    ResolvesEthereumRemoteClientPhishingRoute) {
  GURL url(ethereum_remote_client_phishing_url);
  std::u16string formatted_url = base::UTF8ToUTF16(url.spec());
  HnsLocationBarModelDelegate::FormattedStringFromURL(url, &formatted_url);
  ASSERT_STREQ(base::UTF16ToASCII(formatted_url).c_str(), "hns://wallet");
}
TEST_F(HnsLocationBarModelDelegateTest,
    ResolvesEthereumRemoteClientENSRoute) {
  GURL url(ethereum_remote_client_ens_redirect_url);
  std::u16string formatted_url = base::UTF8ToUTF16(url.spec());
  HnsLocationBarModelDelegate::FormattedStringFromURL(url, &formatted_url);
  ASSERT_STREQ(base::UTF16ToASCII(formatted_url).c_str(), "hns://wallet");
}

TEST_F(HnsLocationBarModelDelegateTest, ResolvesChromeSchemeToHns) {
  GURL url("chrome://sync/");
  std::u16string formatted_url = base::UTF8ToUTF16(url.spec());
  HnsLocationBarModelDelegate::FormattedStringFromURL(url, &formatted_url);
  ASSERT_STREQ(base::UTF16ToASCII(formatted_url).c_str(), "hns://sync/");
}
