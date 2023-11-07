/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/permission_rules/network_connection_permission_rule.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rule_feature.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsNetworkConnectionPermissionRuleTest : public UnitTestBase {
 protected:
  const NetworkConnectionPermissionRule permission_rule_;
};

TEST_F(HnsAdsNetworkConnectionPermissionRuleTest, ShouldAllow) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsNetworkConnectionPermissionRuleTest, ShouldNotAllow) {
  // Arrange
  MockIsNetworkConnectionAvailable(ads_client_mock_, false);

  // Act

  // Assert
  EXPECT_FALSE(permission_rule_.ShouldAllow().has_value());
}

TEST_F(HnsAdsNetworkConnectionPermissionRuleTest,
       ShouldAllowIfPermissionRuleIsDisabled) {
  // Arrange
  base::FieldTrialParams params;
  params["should_only_serve_ads_with_valid_internet_connection"] = "false";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kPermissionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  MockIsNetworkConnectionAvailable(ads_client_mock_, false);

  // Act

  // Assert
  EXPECT_TRUE(permission_rule_.ShouldAllow().has_value());
}

}  // namespace hns_ads
