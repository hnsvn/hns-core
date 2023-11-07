/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/time/time_constraint_util.h"

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsTimeConstraintUtilTest : public UnitTestBase {};

TEST_F(HnsAdsTimeConstraintUtilTest, DoesRespectWhenNoHistoory) {
  // Arrange
  const std::vector<base::Time> history;

  // Act
  const bool does_respect = DoesHistoryRespectRollingTimeConstraint(
      history, /*time_constraint*/ base::Days(1), /*cap*/ 1);

  // Assert
  EXPECT_TRUE(does_respect);
}

TEST_F(HnsAdsTimeConstraintUtilTest, DoesRespect) {
  // Arrange
  std::vector<base::Time> history;
  history.push_back(Now());

  AdvanceClockBy(base::Days(1));

  // Act
  const bool does_respect = DoesHistoryRespectRollingTimeConstraint(
      history, /*time_constraint*/ base::Days(1), /*cap*/ 1);

  // Assert
  EXPECT_TRUE(does_respect);
}

TEST_F(HnsAdsTimeConstraintUtilTest, DoesNotRespect) {
  // Arrange
  std::vector<base::Time> history;
  history.push_back(Now());

  // Act
  const bool does_respect = DoesHistoryRespectRollingTimeConstraint(
      history, /*time_constraint*/ base::Days(1), /*cap*/ 1);

  // Assert
  EXPECT_FALSE(does_respect);
}

}  // namespace hns_ads
