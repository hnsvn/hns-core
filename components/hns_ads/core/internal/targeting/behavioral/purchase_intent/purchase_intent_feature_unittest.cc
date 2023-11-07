/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/purchase_intent_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "base/time/time.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsPurchaseIntentFeatureTest, IsEnabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(IsPurchaseIntentFeatureEnabled());
}

TEST(HnsAdsPurchaseIntentFeatureTest, IsDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kPurchaseIntentFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(IsPurchaseIntentFeatureEnabled());
}

TEST(HnsAdsPurchaseIntentFeatureTest, GetPurchaseIntentResourceVersion) {
  // Arrange
  base::FieldTrialParams params;
  params["resource_version"] = "0";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kPurchaseIntentFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(0, kPurchaseIntentResourceVersion.Get());
}

TEST(HnsAdsPurchaseIntentFeatureTest, DefaultPurchaseIntentResourceVersion) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(1, kPurchaseIntentResourceVersion.Get());
}

TEST(HnsAdsPurchaseIntentFeatureTest,
     DefaultPurchaseIntentResourceVersionWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kPurchaseIntentFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(1, kPurchaseIntentResourceVersion.Get());
}

TEST(HnsAdsPurchaseIntentFeatureTest, GetPurchaseIntentThreshold) {
  // Arrange
  base::FieldTrialParams params;
  params["threshold"] = "5";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kPurchaseIntentFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(5, kPurchaseIntentThreshold.Get());
}

TEST(HnsAdsPurchaseIntentFeatureTest, DefaultPurchaseIntentThreshold) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(3, kPurchaseIntentThreshold.Get());
}

TEST(HnsAdsPurchaseIntentFeatureTest,
     DefaultPurchaseIntentThresholdWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kPurchaseIntentFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(3, kPurchaseIntentThreshold.Get());
}

TEST(HnsAdsPurchaseIntentFeatureTest, GetPurchaseIntentTimeWindow) {
  // Arrange
  base::FieldTrialParams params;
  params["time_window"] = "1d";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kPurchaseIntentFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(base::Days(1), kPurchaseIntentTimeWindow.Get());
}

TEST(HnsAdsPurchaseIntentFeatureTest, DefaultPurchaseIntentTimeWindow) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::Days(7), kPurchaseIntentTimeWindow.Get());
}

TEST(HnsAdsPurchaseIntentFeatureTest,
     DefaultPurchaseIntentTimeWindowWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kPurchaseIntentFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(base::Days(7), kPurchaseIntentTimeWindow.Get());
}

}  // namespace hns_ads
