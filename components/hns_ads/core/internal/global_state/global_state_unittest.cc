/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/global_state/global_state.h"

#include "base/test/task_environment.h"
#include "hns/components/hns_ads/core/internal/client/ads_client_mock.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsGlobalStateTest : public ::testing::Test {
 protected:
  void SetUp() override {
    global_state_ = std::make_unique<GlobalState>(&ads_client_mock_);
  }

  base::test::TaskEnvironment task_environment_;

  ::testing::NiceMock<AdsClientMock> ads_client_mock_;

  std::unique_ptr<GlobalState> global_state_;
};

TEST_F(HnsAdsGlobalStateTest, HasInstance) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(GlobalState::HasInstance());
}

TEST_F(HnsAdsGlobalStateTest, HasNoInstanceAfterDestruction) {
  // Arrange
  global_state_.reset();

  // Act

  // Assert
  EXPECT_FALSE(GlobalState::HasInstance());
}

}  // namespace hns_ads
