/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/feature/notification_ad_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsNotificationAdFeatureTest, IsEnabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(IsNotificationAdFeatureEnabled());
}

TEST(HnsAdsNotificationAdFeatureTest, IsDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kNotificationAdFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(IsNotificationAdFeatureEnabled());
}

TEST(HnsAdsNotificationAdFeatureTest, GetDefaultAdsPerHour) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["default_ads_per_hour"] = "42";
  enabled_features.emplace_back(kNotificationAdFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(42, kDefaultNotificationAdsPerHour.Get());
}

TEST(HnsAdsNotificationAdFeatureTest, DefaultDefaultAdsPerHour) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(10, kDefaultNotificationAdsPerHour.Get());
}

TEST(HnsAdsNotificationAdFeatureTest, DefaultDefaultAdsPerHourWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kNotificationAdFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(10, kDefaultNotificationAdsPerHour.Get());
}

TEST(HnsAdsNotificationAdFeatureTest, GetMaximumAdsPerDay) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["maximum_ads_per_day"] = "24";
  enabled_features.emplace_back(kNotificationAdFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(24, kMaximumNotificationAdsPerDay.Get());
}

TEST(HnsAdsNotificationAdFeatureTest, DefaultMaximumAdsPerDay) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(100, kMaximumNotificationAdsPerDay.Get());
}

TEST(HnsAdsNotificationAdFeatureTest, DefaultMaximumAdsPerDayWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kNotificationAdFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(100, kMaximumNotificationAdsPerDay.Get());
}

TEST(HnsAdsNotificationAdFeatureTest,
     CanFallbackToCustomNotificationAdsDefault) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(false, kCanFallbackToCustomNotificationAds.Get());
}

TEST(HnsAdsNotificationAdFeatureTest, CanFallbackToCustomNotificationAds) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["can_fallback_to_custom_notifications"] = "true";
  enabled_features.emplace_back(kNotificationAdFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(true, kCanFallbackToCustomNotificationAds.Get());
}

}  // namespace hns_ads
