/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/url_requests/get_signed_tokens/get_signed_tokens_url_request_builder_util.h"

#include "hns/components/hns_ads/core/internal/account/utility/refill_confirmation_tokens/url_requests/get_signed_tokens/get_signed_tokens_url_request_builder_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/account/wallet/wallet_unittest_constants.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsBuildGetSignedTokensUrlRequestBuilderUtilTest, GetPath) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(
      R"(/v3/confirmation/token/27a39b2f-9b2e-4eb0-bbb2-2f84447496e7?nonce=2f0e2891-e7a5-4262-835b-550b13e58e5c)",
      BuildGetSignedTokensUrlPath(kWalletPaymentId, kGetSignedTokensNonce));
}

}  // namespace hns_ads
