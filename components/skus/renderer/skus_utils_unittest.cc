/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/skus/renderer/skus_utils.h"

#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/blink/public/platform/web_security_origin.h"
#include "third_party/blink/public/platform/web_url.h"
#include "url/gurl.h"

namespace skus {

TEST(SkusUtilsUnitTest, IsSafeOrigin) {
  EXPECT_TRUE(skus::IsSafeOrigin(
      blink::WebSecurityOrigin::Create(GURL("https://account.hns.com"))));
  EXPECT_TRUE(skus::IsSafeOrigin(blink::WebSecurityOrigin::Create(
      GURL("https://account.hnssoftware.com"))));
  EXPECT_TRUE(skus::IsSafeOrigin(blink::WebSecurityOrigin::Create(
      GURL("https://account.hns.software"))));

  EXPECT_FALSE(skus::IsSafeOrigin(blink::WebSecurityOrigin::Create(
      GURL("https://a.account.hns.software"))));
  EXPECT_FALSE(skus::IsSafeOrigin(
      blink::WebSecurityOrigin::Create(GURL("https://vpm.hns.com"))));
  EXPECT_FALSE(skus::IsSafeOrigin(blink::WebSecurityOrigin::Create(GURL())));
  EXPECT_FALSE(skus::IsSafeOrigin(
      blink::WebSecurityOrigin::Create(GURL("http://account.hns.com"))));
  EXPECT_FALSE(skus::IsSafeOrigin(
      blink::WebSecurityOrigin::Create(GURL("https://hns.com"))));
  EXPECT_FALSE(skus::IsSafeOrigin(
      blink::WebSecurityOrigin::Create(GURL("file://hns.com"))));
}

}  // namespace skus
