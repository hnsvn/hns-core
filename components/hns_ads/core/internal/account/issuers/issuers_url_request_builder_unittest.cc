/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/issuers/issuers_url_request_builder.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "url/gurl.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsIssuersUrlRequestBuilderTest : public UnitTestBase {};

TEST_F(HnsAdsIssuersUrlRequestBuilderTest, BuildUrl) {
  // Arrange
  IssuersUrlRequestBuilder url_request_builder;

  // Act
  const mojom::UrlRequestInfoPtr url_request = url_request_builder.Build();

  // Assert
  mojom::UrlRequestInfoPtr expected_url_request = mojom::UrlRequestInfo::New();
  expected_url_request->url =
      GURL("https://static.ads.hnssoftware.com/v3/issuers");
  expected_url_request->method = mojom::UrlRequestMethodType::kGet;

  EXPECT_EQ(expected_url_request, url_request);
}

}  // namespace hns_ads
