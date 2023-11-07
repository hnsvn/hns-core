/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/split_test_exclusion_rule.h"

#include "base/metrics/field_trial.h"
#include "base/test/mock_entropy_provider.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

constexpr char kTrialName[] = "AdvertiserSplitTestStudy";
constexpr char kGroupName[] = "GroupA";

scoped_refptr<base::FieldTrial> CreateFieldTrial(
    const std::string& trial_name) {
  base::MockEntropyProvider entropy_provider(0.9);
  return base::FieldTrialList::FactoryGetFieldTrial(
      trial_name, /*total_probability*/ 100, "default_group_name",
      entropy_provider);
}

}  // namespace

class HnsAdsSplitTestExclusionRuleTest : public UnitTestBase {
 protected:
  const SplitTestExclusionRule exclusion_rule_;
};

TEST_F(HnsAdsSplitTestExclusionRuleTest, AllowIfNoFieldTrialAndNoAdGroup) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_set_id = kCreativeSetId;

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule_.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsSplitTestExclusionRuleTest, DoNotAllowIfNoFieldTrialAndAdGroup) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_set_id = kCreativeSetId;
  creative_ad.split_test_group = "GroupA";

  // Act

  // Assert
  EXPECT_FALSE(exclusion_rule_.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsSplitTestExclusionRuleTest, AllowIfFieldTrialAndNoAdGroup) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_set_id = kCreativeSetId;

  const scoped_refptr<base::FieldTrial> field_trial =
      CreateFieldTrial(kTrialName);
  field_trial->AppendGroup(kGroupName, /*group_probability*/ 100);

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule_.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsSplitTestExclusionRuleTest, AllowIfFieldTrialMatchesAdGroup) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_set_id = kCreativeSetId;
  creative_ad.split_test_group = "GroupA";

  const scoped_refptr<base::FieldTrial> field_trial =
      CreateFieldTrial(kTrialName);
  field_trial->AppendGroup(kGroupName, /*group_probability*/ 100);

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule_.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsSplitTestExclusionRuleTest,
       DoNotAllowIfFieldTrialDoesNotMatchAdGroup) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_set_id = kCreativeSetId;
  creative_ad.split_test_group = "GroupB";

  const scoped_refptr<base::FieldTrial> field_trial =
      CreateFieldTrial(kTrialName);
  field_trial->AppendGroup(kGroupName, /*group_probability*/ 100);

  // Act

  // Assert
  EXPECT_FALSE(exclusion_rule_.ShouldInclude(creative_ad).has_value());
}

}  // namespace hns_ads
