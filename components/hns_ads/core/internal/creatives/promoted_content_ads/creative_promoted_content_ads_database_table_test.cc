/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/creatives/promoted_content_ads/creative_promoted_content_ads_database_table.h"

#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_url_response_alias.h"
#include "net/http/http_status_code.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCreativePromotedContentAdsDatabaseTableIntegrationTest
    : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUpForTesting(/*is_integration_test*/ true);
  }

  void SetUpMocks() override {
    const URLResponseMap url_responses = {
        {BuildCatalogUrlPath(),
         {{net::HTTP_OK, /*response_body*/ "/catalog.json"}}}};
    MockUrlResponses(ads_client_mock_, url_responses);
  }
};

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableIntegrationTest,
       GetCreativePromotedContentAdsFromCatalogResponse) {
  // Arrange

  // Act

  // Assert
  const database::table::CreativePromotedContentAds database_table;
  database_table.GetForSegments(
      /*segments*/ {"technology & computing"},
      base::BindOnce([](const bool success, const SegmentList& /*segments*/,
                        const CreativePromotedContentAdList&
                            creative_promoted_content_ads) {
        EXPECT_TRUE(success);
        EXPECT_EQ(1U, creative_promoted_content_ads.size());
      }));
}

}  // namespace hns_ads
