/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/url/request_builder/host/url_host_util.h"
#include "hns/components/hns_ads/core/internal/global_state/global_state.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsGeoUrlHostTest : public UnitTestBase {};

TEST_F(HnsAdsGeoUrlHostTest, GetProductionUrlHost) {
  // Arrange
  GlobalState::GetInstance()->Flags().environment_type =
      mojom::EnvironmentType::kProduction;

  // Act

  // Assert
  EXPECT_EQ("https://geo.ads.hns.com", GetGeoUrlHost());
}

TEST_F(HnsAdsGeoUrlHostTest, GetStagingUrlHost) {
  // Arrange
  GlobalState::GetInstance()->Flags().environment_type =
      mojom::EnvironmentType::kStaging;

  // Act

  // Assert
  EXPECT_EQ("https://geo.ads.hnssoftware.com", GetGeoUrlHost());
}

}  // namespace hns_ads
