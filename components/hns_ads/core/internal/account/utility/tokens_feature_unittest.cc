/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/tokens_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsTokensFeatureTest, MinConfirmationTokens) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["min_confirmation_tokens"] = "7";
  enabled_features.emplace_back(kAccountTokensFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(7, kMinConfirmationTokens.Get());
}

TEST(HnsAdsTokensFeatureTest, DefaultMinConfirmationTokens) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(20, kMinConfirmationTokens.Get());
}

TEST(HnsAdsTokensFeatureTest, DefaultMinConfirmationTokensWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kAccountTokensFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(20, kMinConfirmationTokens.Get());
}

TEST(HnsAdsTokensFeatureTest, MaxConfirmationTokens) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["max_confirmation_tokens"] = "21";
  enabled_features.emplace_back(kAccountTokensFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(21, kMaxConfirmationTokens.Get());
}

TEST(HnsAdsTokensFeatureTest, DefaultMaxConfirmationTokens) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(50, kMaxConfirmationTokens.Get());
}

TEST(HnsAdsTokensFeatureTest, DefaultMaxConfirmationTokensWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kAccountTokensFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(50, kMaxConfirmationTokens.Get());
}

}  // namespace hns_ads
