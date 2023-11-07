/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/feature/hns_ads_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsHnsAdsFeatureTest, ShouldLaunchAsInProcessService) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldLaunchHnsAdsAsAnInProcessServiceFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_TRUE(ShouldLaunchAsInProcessService());
}

TEST(HnsAdsHnsAdsFeatureTest, ShouldNotLaunchAsInProcessService) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ShouldLaunchAsInProcessService());
}

TEST(HnsAdsHnsAdsFeatureTest, ShouldAlwaysRunService) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(kShouldAlwaysRunHnsAdsServiceFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_TRUE(ShouldAlwaysRunService());
}

TEST(HnsAdsHnsAdsFeatureTest, ShouldNotAlwaysRunService) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ShouldAlwaysRunService());
}

TEST(HnsAdsHnsAdsFeatureTest, ShouldAlwaysTriggerNewTabPageAdEvents) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsNewTabPageAdEventsFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_TRUE(ShouldAlwaysTriggerNewTabPageAdEvents());
}

TEST(HnsAdsHnsAdsFeatureTest, ShouldNotAlwaysTriggerNewTabPageAdEvents) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ShouldAlwaysTriggerNewTabPageAdEvents());
}

TEST(HnsAdsHnsAdsFeatureTest, ShouldSupportSearchResultAds) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(kShouldSupportSearchResultAdsFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_TRUE(ShouldSupportSearchResultAds());
}

TEST(HnsAdsHnsAdsFeatureTest, ShouldNotSupportSearchResultAds) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ShouldSupportSearchResultAds());
}

TEST(HnsAdsHnsAdsFeatureTest, ShouldAlwaysTriggerSearchResultAdEvents) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsSearchResultAdEventsFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_TRUE(ShouldAlwaysTriggerSearchResultAdEvents());
}

TEST(HnsAdsHnsAdsFeatureTest, ShouldNotAlwaysTriggerSearchResultAdEvents) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ShouldAlwaysTriggerSearchResultAdEvents());
}

}  // namespace hns_ads
