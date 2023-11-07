/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/url/request_builder/host/url_host_util.h"
#include "hns/components/hns_ads/core/internal/global_state/global_state.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsAnonymousSearchUrlHostTest : public UnitTestBase {};

TEST_F(HnsAdsAnonymousSearchUrlHostTest, GetProductionUrlHost) {
  // Arrange
  GlobalState::GetInstance()->Flags().environment_type =
      mojom::EnvironmentType::kProduction;

  // Act

  // Assert
  EXPECT_EQ("https://search.anonymous.ads.hns.com",
            GetAnonymousSearchUrlHost());
}

TEST_F(HnsAdsAnonymousSearchUrlHostTest, GetStagingUrlHost) {
  // Arrange
  GlobalState::GetInstance()->Flags().environment_type =
      mojom::EnvironmentType::kStaging;

  // Act

  // Assert
  EXPECT_EQ("https://search.anonymous.ads.hnssoftware.com",
            GetAnonymousSearchUrlHost());
}

}  // namespace hns_ads
