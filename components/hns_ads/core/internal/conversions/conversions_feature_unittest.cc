/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/conversions_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsConversionsFeatureTest, IsEnabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(IsConversionFeatureEnabled());
}

TEST(HnsAdsConversionsFeatureTest, IsDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kConversionsFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(IsConversionFeatureEnabled());
}

TEST(HnsAdsConversionsFeatureTest, GetConversionResourceVersion) {
  // Arrange
  base::FieldTrialParams params;
  params["resource_version"] = "0";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kConversionsFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(0, kConversionResourceVersion.Get());
}

TEST(HnsAdsConversionsFeatureTest, DefaultConversionResourceVersion) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(1, kConversionResourceVersion.Get());
}

TEST(HnsAdsConversionsFeatureTest,
     DefaultConversionResourceVersionWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kConversionsFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(1, kConversionResourceVersion.Get());
}

TEST(HnsAdsConversionsFeatureTest, GetConversionIdPattern) {
  // Arrange
  base::FieldTrialParams params;
  params["html_meta_tag_id_pattern"] = "*";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kConversionsFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ("*", kHtmlMetaTagConversionIdPattern.Get());
}

TEST(HnsAdsConversionsFeatureTest, DefaultConversionIdPattern) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(R"~(<meta.*name="ad-conversion-id".*content="([-a-zA-Z0-9]*)".*>)~",
            kHtmlMetaTagConversionIdPattern.Get());
}

TEST(HnsAdsConversionsFeatureTest, DefaultConversionIdPatternWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kConversionsFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(R"~(<meta.*name="ad-conversion-id".*content="([-a-zA-Z0-9]*)".*>)~",
            kHtmlMetaTagConversionIdPattern.Get());
}

}  // namespace hns_ads
