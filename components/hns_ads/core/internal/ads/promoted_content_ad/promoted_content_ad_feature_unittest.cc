/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ads/promoted_content_ad/promoted_content_ad_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsPromotedContentAdFeatureTest, IsEnabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(IsPromotedContentAdFeatureEnabled());
}

TEST(HnsAdsPromotedContentAdFeatureTest, IsDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kPromotedContentAdFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(IsPromotedContentAdFeatureEnabled());
}

TEST(HnsAdsPromotedContentAdFeatureTest, GetMaximumAdsPerHour) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["maximum_ads_per_hour"] = "42";
  enabled_features.emplace_back(kPromotedContentAdFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(42, kMaximumPromotedContentAdsPerHour.Get());
}

TEST(HnsAdsPromotedContentAdFeatureTest, DefaultMaximumAdsPerHour) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(4, kMaximumPromotedContentAdsPerHour.Get());
}

TEST(HnsAdsPromotedContentAdFeatureTest,
     DefaultMaximumAdsPerHourWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kPromotedContentAdFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(4, kMaximumPromotedContentAdsPerHour.Get());
}

TEST(HnsAdsPromotedContentAdFeatureTest, GetMaximumAdsPerDay) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["maximum_ads_per_day"] = "24";
  enabled_features.emplace_back(kPromotedContentAdFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(24, kMaximumPromotedContentAdsPerDay.Get());
}

TEST(HnsAdsPromotedContentAdFeatureTest, DefaultMaximumAdsPerDay) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(20, kMaximumPromotedContentAdsPerDay.Get());
}

TEST(HnsAdsPromotedContentAdFeatureTest,
     DefaultMaximumAdsPerDayWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kPromotedContentAdFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(20, kMaximumPromotedContentAdsPerDay.Get());
}

}  // namespace hns_ads
