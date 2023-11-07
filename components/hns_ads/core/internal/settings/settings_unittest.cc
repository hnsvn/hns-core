/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/settings/settings.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_pref_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/public/feature/notification_ad_feature.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsSettingsTest : public UnitTestBase {};

TEST_F(HnsAdsSettingsTest, UserHasJoinedHnsRewards) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(UserHasJoinedHnsRewards());
}

TEST_F(HnsAdsSettingsTest, UserHasNotJoinedHnsRewards) {
  // Arrange
  DisableHnsRewardsForTesting();

  // Act

  // Assert
  EXPECT_FALSE(UserHasJoinedHnsRewards());
}

TEST_F(HnsAdsSettingsTest, UserHasOptedInToHnsNewsAds) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(UserHasOptedInToHnsNewsAds());
}

TEST_F(HnsAdsSettingsTest, UserHasNotOptedInToHnsNews) {
  // Arrange
  DisableHnsNewsAdsForTesting();

  // Act

  // Assert
  EXPECT_FALSE(UserHasOptedInToHnsNewsAds());
}

TEST_F(HnsAdsSettingsTest, UserHasOptedInToNewTabPageAds) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(UserHasOptedInToNewTabPageAds());
}

TEST_F(HnsAdsSettingsTest, UserHasNotOptedInToNewTabPageAds) {
  // Arrange
  DisableNewTabPageAdsForTesting();

  // Act

  // Assert
  EXPECT_FALSE(UserHasOptedInToNewTabPageAds());
}

TEST_F(HnsAdsSettingsTest, UserHasOptedInToNotificationAds) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(UserHasOptedInToNotificationAds());
}

TEST_F(HnsAdsSettingsTest, UserHasNotOptedInToNotificationAds) {
  // Arrange
  DisableNotificationAdsForTesting();

  // Act

  // Assert
  EXPECT_FALSE(UserHasOptedInToNotificationAds());
}

TEST_F(HnsAdsSettingsTest, MaximumNotificationAdsPerHour) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["default_ads_per_hour"] = "2";
  enabled_features.emplace_back(kNotificationAdFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  SetInt64Pref(prefs::kMaximumNotificationAdsPerHour, 3);

  // Act

  // Assert
  EXPECT_EQ(3, GetMaximumNotificationAdsPerHour());
}

TEST_F(HnsAdsSettingsTest, DefaultMaximumNotificationAdsPerHour) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["default_ads_per_hour"] = "2";
  enabled_features.emplace_back(kNotificationAdFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act

  // Assert
  EXPECT_EQ(2, GetMaximumNotificationAdsPerHour());
}

}  // namespace hns_ads
