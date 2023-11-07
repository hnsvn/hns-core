/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/analytics/p2a/opportunities/p2a_opportunity_util.h"

#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::p2a {

TEST(HnsAdsP2AOpportunityUtilTest, BuildAdOpportunityEvents) {
  // Arrange

  // Act

  // Assert
  const std::vector<std::string> expected_events = {
      "Hns.P2A.AdOpportunitiesPerSegment.technologycomputing",
      "Hns.P2A.AdOpportunitiesPerSegment.personalfinance",
      "Hns.P2A.AdOpportunitiesPerSegment.travel",
      "Hns.P2A.TotalAdOpportunities"};

  EXPECT_EQ(expected_events, BuildAdOpportunityEvents(/*segments*/ {
                                 "technology & computing",
                                 "personal finance-crypto", "travel"}));
}

TEST(HnsAdsP2AOpportunityUtilTest, BuildAdOpportunityEventsForEmptySegments) {
  // Arrange

  // Act

  // Assert
  const std::vector<std::string> expected_events = {
      "Hns.P2A.TotalAdOpportunities"};

  EXPECT_EQ(expected_events, BuildAdOpportunityEvents(/*segments*/ {}));
}

}  // namespace hns_ads::p2a
