/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/ads/new_tab_page_ad_info.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/new_tab_page_ads/creative_new_tab_page_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/new_tab_page_ads/creative_new_tab_page_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/new_tab_page_ads/new_tab_page_ad_builder.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsNewTabPageAdInfoTest : public UnitTestBase {};

TEST_F(HnsAdsNewTabPageAdInfoTest, IsValid) {
  // Arrange
  const CreativeNewTabPageAdInfo creative_ad =
      BuildCreativeNewTabPageAdForTesting(/*should_use_random_uuids*/ true);
  const NewTabPageAdInfo ad = BuildNewTabPageAd(creative_ad);

  // Act

  // Assert
  EXPECT_TRUE(ad.IsValid());
}

TEST_F(HnsAdsNewTabPageAdInfoTest, IsInvalid) {
  // Arrange
  const NewTabPageAdInfo ad;

  // Act

  // Assert
  EXPECT_FALSE(ad.IsValid());
}

}  // namespace hns_ads
