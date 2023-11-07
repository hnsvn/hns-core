/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/dislike_exclusion_rule.h"

#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/client_state_manager.h"
#include "hns/components/hns_ads/core/public/history/ad_content_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsDislikeExclusionRuleTest : public UnitTestBase {
 protected:
  const DislikeExclusionRule exclusion_rule_;
};

TEST_F(HnsAdsDislikeExclusionRuleTest, ShouldInclude) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.advertiser_id = kAdvertiserId;

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule_.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDislikeExclusionRuleTest, ShouldExclude) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.advertiser_id = kAdvertiserId;

  AdContentInfo ad_content;
  ad_content.advertiser_id = kAdvertiserId;
  ad_content.user_reaction_type = mojom::UserReactionType::kNeutral;
  ClientStateManager::GetInstance().ToggleDislikeAd(ad_content);

  // Act

  // Assert
  EXPECT_FALSE(exclusion_rule_.ShouldInclude(creative_ad).has_value());
}

}  // namespace hns_ads
