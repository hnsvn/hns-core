/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/reminder/reminder_feature.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsReminderFeatureTest, IsEnabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(IsReminderFeatureEnabled());
}

TEST(HnsAdsReminderFeatureTest, IsDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kReminderFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_FALSE(IsReminderFeatureEnabled());
}

TEST(HnsAdsReminderFeatureTest, RemindUserIfClickingTheSameAdAfter) {
  // Arrange
  base::FieldTrialParams params;
  params["remind_user_if_clicking_the_same_ad_after"] = "1";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kReminderFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(1, kRemindUserIfClickingTheSameAdAfter.Get());
}

TEST(HnsAdsReminderFeatureTest, DefaultRemindUserIfClickingTheSameAdAfter) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(3, kRemindUserIfClickingTheSameAdAfter.Get());
}

TEST(HnsAdsReminderFeatureTest,
     DefaultRemindUserIfClickingTheSameAdAfterWhenDisabled) {
  // Arrange
  const std::vector<base::test::FeatureRefAndParams> enabled_features;

  std::vector<base::test::FeatureRef> disabled_features;
  disabled_features.emplace_back(kReminderFeature);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(3, kRemindUserIfClickingTheSameAdAfter.Get());
}

}  // namespace hns_ads
