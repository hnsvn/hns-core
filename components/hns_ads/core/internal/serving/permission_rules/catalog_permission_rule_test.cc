/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/catalog_permission_rule.h"

#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "net/http/http_status_code.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCatalogPermissionRuleIntegrationTest : public UnitTestBase {
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

  const CatalogPermissionRule permission_rule_;
};

TEST_F(HnsAdsCatalogPermissionRuleIntegrationTest, ShouldAllow) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsCatalogPermissionRuleIntegrationTest,
       ShouldAllowIfCatalogWasLastUpdated23HoursAnd59MinutesAgo) {
  // Arrange
  AdvanceClockBy(base::Days(1) - base::Milliseconds(1));

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsCatalogPermissionRuleIntegrationTest,
       ShouldNotAllowIfCatalogWasLastUpdated1DayAgo) {
  // Arrange
  AdvanceClockBy(base::Days(1));

  // Act

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsCatalogPermissionRuleIntegrationTest,
       ShouldNotAllowIfCatalogDoesNotExist) {
  // Arrange
  SetCatalogVersion(0);

  // Act

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

}  // namespace hns_ads
