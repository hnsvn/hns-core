/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/url_requests/request_signed_tokens/request_signed_tokens_url_request_util.h"

#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/refill_confirmation_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/url_requests/get_signed_tokens/get_signed_tokens_url_request_builder_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsRequestSignedTokensUrlRequestUtilTest : public UnitTestBase {};

TEST_F(HnsAdsRequestSignedTokensUrlRequestUtilTest, ParseNonce) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(kGetSignedTokensNonce,
            ParseNonce(base::test::ParseJsonDict(
                BuildRequestSignedTokensUrlResponseBodyForTesting())));
}

TEST_F(HnsAdsRequestSignedTokensUrlRequestUtilTest, DoNotParseMissingNonce) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ParseNonce(base::test::ParseJsonDict("{}")));
}

}  // namespace hns_ads
