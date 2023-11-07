/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/creatives/conversions/creative_set_conversion_database_table.h"

#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "net/http/http_status_code.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsConversionsDatabaseTableIntegrationTest : public UnitTestBase {
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

TEST_F(HnsAdsConversionsDatabaseTableIntegrationTest,
       GetConversionsFromCatalogResponse) {
  // Arrange

  // Act

  // Assert
  const database::table::CreativeSetConversions database_table;
  database_table.GetAll(base::BindOnce(
      [](const bool success,
         const CreativeSetConversionList& creative_set_conversions) {
        EXPECT_TRUE(success);
        EXPECT_EQ(2U, creative_set_conversions.size());
      }));
}

}  // namespace hns_ads
