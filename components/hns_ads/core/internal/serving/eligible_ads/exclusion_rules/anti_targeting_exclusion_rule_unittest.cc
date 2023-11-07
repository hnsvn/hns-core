/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/anti_targeting_exclusion_rule.h"

#include <memory>

#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/resources/country_components_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/anti_targeting/resource/anti_targeting_resource.h"
#include "url/gurl.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsAntiTargetingExclusionRuleTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    resource_ = std::make_unique<AntiTargetingResource>();
  }

  bool LoadResource() {
    NotifyDidUpdateResourceComponent(kCountryComponentManifestVersion,
                                     kCountryComponentId);
    task_environment_.RunUntilIdle();
    return resource_->IsInitialized();
  }

  std::unique_ptr<AntiTargetingResource> resource_;
};

TEST_F(HnsAdsAntiTargetingExclusionRuleTest,
       AllowIfResourceIsNotInitialized) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_set_id = kCreativeSetId;

  const BrowsingHistoryList history = {GURL("https://www.foo1.org"),
                                       GURL("https://www.hns.com"),
                                       GURL("https://www.foo2.org")};

  const AntiTargetingExclusionRule exclusion_rule(*resource_, history);

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsAntiTargetingExclusionRuleTest, AllowIfCreativeSetDoesNotExist) {
  // Arrange
  ASSERT_TRUE(LoadResource());

  const BrowsingHistoryList history = {GURL("https://www.foo1.org"),
                                       GURL("https://www.hns.com"),
                                       GURL("https://www.foo2.org")};

  const AntiTargetingExclusionRule exclusion_rule(*resource_, history);

  // Act

  // Assert
  CreativeAdInfo creative_ad;
  creative_ad.creative_set_id = kMissingCreativeSetId;
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsAntiTargetingExclusionRuleTest, AllowIfSiteDoesNotExist) {
  // Arrange
  ASSERT_TRUE(LoadResource());

  const BrowsingHistoryList history = {GURL("https://www.foo1.org"),
                                       GURL("https://www.foo2.org")};

  const AntiTargetingExclusionRule exclusion_rule(*resource_, history);

  // Act

  // Assert
  CreativeAdInfo creative_ad;
  creative_ad.creative_set_id = kCreativeSetId;
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsAntiTargetingExclusionRuleTest,
       DoNotAllowIfCreativeSetAndSiteMatch) {
  // Arrange
  ASSERT_TRUE(LoadResource());

  const BrowsingHistoryList history = {GURL("https://www.foo1.org"),
                                       GURL("https://www.hns.com")};

  const AntiTargetingExclusionRule exclusion_rule(*resource_, history);

  // Act

  // Assert
  CreativeAdInfo creative_ad;
  creative_ad.creative_set_id = kCreativeSetId;
  EXPECT_FALSE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

}  // namespace hns_ads
