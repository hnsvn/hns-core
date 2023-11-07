/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/feature/search_result_ad_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsSearchResultAdFeatureTest, GetMaximumAdsPerHour) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["maximum_ads_per_hour"] = "42";
  enabled_features.emplace_back(kSearchResultAdFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(42, kMaximumSearchResultAdsPerHour.Get());
}

TEST(HnsAdsSearchResultAdFeatureTest, DefaultMaximumAdsPerHour) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(10, kMaximumSearchResultAdsPerHour.Get());
}

TEST(HnsAdsSearchResultAdFeatureTest, DefaultMaximumAdsPerHourWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kSearchResultAdFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(10, kMaximumSearchResultAdsPerHour.Get());
}

TEST(HnsAdsSearchResultAdFeatureTest, GetMaximumAdsPerDay) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["maximum_ads_per_day"] = "42";
  enabled_features.emplace_back(kSearchResultAdFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(42, kMaximumSearchResultAdsPerDay.Get());
}

TEST(HnsAdsSearchResultAdFeatureTest, DefaultMaximumAdsPerDay) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(40, kMaximumSearchResultAdsPerDay.Get());
}

TEST(HnsAdsSearchResultAdFeatureTest, DefaultMaximumAdsPerDayWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kSearchResultAdFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(40, kMaximumSearchResultAdsPerDay.Get());
}

}  // namespace hns_ads
