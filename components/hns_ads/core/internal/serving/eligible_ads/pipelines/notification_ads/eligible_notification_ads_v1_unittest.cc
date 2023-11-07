/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/pipelines/notification_ads/eligible_notification_ads_v1.h"

#include <memory>
#include <utility>

#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_container_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ads_database_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/notification_ad_builder.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/client_state_manager.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/pacing/pacing_random_util.h"
#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_builder_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_info.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/anti_targeting/resource/anti_targeting_resource.h"
#include "hns/components/hns_ads/core/internal/targeting/geographical/subdivision/subdivision_targeting.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsEligibleNotificationAdsV1Test : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    subdivision_targeting_ = std::make_unique<SubdivisionTargeting>();
    anti_targeting_resource_ = std::make_unique<AntiTargetingResource>();
    eligible_ads_ = std::make_unique<EligibleNotificationAdsV1>(
        *subdivision_targeting_, *anti_targeting_resource_);
  }

  std::unique_ptr<SubdivisionTargeting> subdivision_targeting_;
  std::unique_ptr<AntiTargetingResource> anti_targeting_resource_;
  std::unique_ptr<EligibleNotificationAdsV1> eligible_ads_;
};

TEST_F(HnsAdsEligibleNotificationAdsV1Test, GetAdsForChildSegment) {
  // Arrange
  CreativeNotificationAdList creative_ads;

  CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_1.segment = "technology & computing";
  creative_ads.push_back(creative_ad_1);

  CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_2.segment = "technology & computing-software";
  creative_ads.push_back(creative_ad_2);

  database::SaveCreativeNotificationAds(creative_ads);

  // Act
  CreativeNotificationAdList expected_creative_ads = {creative_ad_2};

  eligible_ads_->GetForUserModel(
      BuildUserModelForTesting(
          /*intent_segments*/ {},
          /*latent_interest_segments*/ {},
          /*interest_segments*/ {"technology & computing-software"},
          /*text_embedding_html_events*/ {}),
      base::BindOnce(
          [](const CreativeNotificationAdList& expected_creative_ads,
             const bool had_opportunity,
             const CreativeNotificationAdList& creative_ads) {
            // Assert
            EXPECT_TRUE(had_opportunity);
            EXPECT_EQ(expected_creative_ads, creative_ads);
          },
          std::move(expected_creative_ads)));
}

TEST_F(HnsAdsEligibleNotificationAdsV1Test, GetAdsForParentSegment) {
  // Arrange
  CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad.segment = "technology & computing";
  database::SaveCreativeNotificationAds({creative_ad});

  // Act
  CreativeNotificationAdList expected_creative_ads = {creative_ad};

  eligible_ads_->GetForUserModel(
      BuildUserModelForTesting(
          /*intent_segments*/ {},
          /*latent_interest_segments*/ {},
          /*interest_segments*/ {"technology & computing-software"},
          /*text_embedding_html_events*/ {}),
      base::BindOnce(
          [](const CreativeNotificationAdList& expected_creative_ads,
             const bool had_opportunity,
             const CreativeNotificationAdList& creative_ads) {
            // Assert
            EXPECT_TRUE(had_opportunity);
            EXPECT_EQ(expected_creative_ads, creative_ads);
          },
          std::move(expected_creative_ads)));
}

TEST_F(HnsAdsEligibleNotificationAdsV1Test, GetAdsForUntargetedSegment) {
  // Arrange
  CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad.segment = "untargeted";
  database::SaveCreativeNotificationAds({creative_ad});

  // Act
  CreativeNotificationAdList expected_creative_ads = {creative_ad};

  eligible_ads_->GetForUserModel(
      BuildUserModelForTesting(
          /*intent_segments*/ {},
          /*latent_interest_segments*/ {},
          /*interest_segments*/ {"finance-banking"},
          /*text_embedding_html_events*/ {}),
      base::BindOnce(
          [](const CreativeNotificationAdList& expected_creative_ads,
             const bool had_opportunity,
             const CreativeNotificationAdList& creative_ads) {
            // Assert
            EXPECT_TRUE(had_opportunity);
            EXPECT_EQ(expected_creative_ads, creative_ads);
          },
          std::move(expected_creative_ads)));
}

TEST_F(HnsAdsEligibleNotificationAdsV1Test, GetAdsForMultipleSegments) {
  // Arrange
  CreativeNotificationAdList creative_ads;

  CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_1.segment = "technology & computing";
  creative_ads.push_back(creative_ad_1);

  CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_2.segment = "finance-banking";
  creative_ads.push_back(creative_ad_2);

  CreativeNotificationAdInfo creative_ad_3 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_3.segment = "food & drink";
  creative_ads.push_back(creative_ad_3);

  database::SaveCreativeNotificationAds(creative_ads);

  // Act
  CreativeNotificationAdList expected_creative_ads = {creative_ad_1,
                                                      creative_ad_3};

  eligible_ads_->GetForUserModel(
      BuildUserModelForTesting(
          /*intent_segments*/ {},
          /*latent_interest_segments*/ {},
          /*interest_segments*/ {"technology & computing", "food & drink"},
          /*text_embedding_html_events*/ {}),
      base::BindOnce(
          [](const CreativeNotificationAdList& expected_creative_ads,
             const bool had_opportunity,
             const CreativeNotificationAdList& creative_ads) {
            // Assert
            EXPECT_TRUE(had_opportunity);
            EXPECT_TRUE(ContainersEq(expected_creative_ads, creative_ads));
          },
          std::move(expected_creative_ads)));
}

TEST_F(HnsAdsEligibleNotificationAdsV1Test, GetAdsForNoSegments) {
  // Arrange
  CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad.segment = "untargeted";
  database::SaveCreativeNotificationAds({creative_ad});

  // Act
  CreativeNotificationAdList expected_creative_ads = {creative_ad};

  eligible_ads_->GetForUserModel(
      /*user_model*/ {},
      base::BindOnce(
          [](const CreativeNotificationAdList& expected_creative_ads,
             const bool had_opportunity,
             const CreativeNotificationAdList& creative_ads) {
            // Assert
            EXPECT_TRUE(had_opportunity);
            EXPECT_EQ(expected_creative_ads, creative_ads);
          },
          std::move(expected_creative_ads)));
}

TEST_F(HnsAdsEligibleNotificationAdsV1Test, DoNotGetAdsForUnmatchedSegments) {
  // Arrange
  CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad.segment = "technology & computing";
  database::SaveCreativeNotificationAds({creative_ad});

  // Act
  eligible_ads_->GetForUserModel(
      BuildUserModelForTesting(
          /*intent_segments*/ {},
          /*latent_interest_segments*/ {},
          /*interest_segments*/ {"UNMATCHED"},
          /*text_embedding_html_events*/ {}),
      base::BindOnce([](const bool had_opportunity,
                        const CreativeNotificationAdList& creative_ads) {
        // Assert
        EXPECT_FALSE(had_opportunity);
        EXPECT_TRUE(creative_ads.empty());
      }));
}

TEST_F(HnsAdsEligibleNotificationAdsV1Test, DoNotGetAdsIfNoEligibleAds) {
  // Arrange

  // Act
  eligible_ads_->GetForUserModel(
      BuildUserModelForTesting(
          /*intent_segments*/ {},
          /*latent_interest_segments*/ {},
          /*interest_segments*/ {"technology & computing", "food & drink"},
          /*text_embedding_html_events*/ {}),
      base::BindOnce([](const bool had_opportunity,
                        const CreativeNotificationAdList& creative_ads) {
        // Assert
        EXPECT_FALSE(had_opportunity);
        EXPECT_TRUE(creative_ads.empty());
      }));
}

TEST_F(HnsAdsEligibleNotificationAdsV1Test, DoNotGetAdsIfAlreadySeen) {
  // Arrange
  CreativeNotificationAdList creative_ads;

  CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_1.segment = "technology & computing";
  creative_ads.push_back(creative_ad_1);

  CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_2.segment = "food & drink";
  creative_ads.push_back(creative_ad_2);

  database::SaveCreativeNotificationAds(creative_ads);

  const NotificationAdInfo ad = BuildNotificationAd(creative_ad_1);
  ClientStateManager::GetInstance().UpdateSeenAd(ad);

  // Act
  CreativeNotificationAdList expected_creative_ads = {creative_ad_2};

  eligible_ads_->GetForUserModel(
      BuildUserModelForTesting(
          /*intent_segments*/ {},
          /*latent_interest_segments*/ {},
          /*interest_segments*/ {"technology & computing", "food & drink"},
          /*text_embedding_html_events*/ {}),
      base::BindOnce(
          [](const CreativeNotificationAdList& expected_creative_ads,
             const bool had_opportunity,
             const CreativeNotificationAdList& creative_ads) {
            // Assert
            EXPECT_TRUE(had_opportunity);
            EXPECT_EQ(expected_creative_ads, creative_ads);
          },
          std::move(expected_creative_ads)));
}

TEST_F(HnsAdsEligibleNotificationAdsV1Test, DoNotGetPacedAds) {
  // Arrange
  CreativeNotificationAdList creative_ads;

  CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_1.segment = "technology & computing";
  creative_ad_1.pass_through_rate = 0.1;
  creative_ads.push_back(creative_ad_1);

  CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_2.segment = "food & drink";
  creative_ad_2.pass_through_rate = 0.5;
  creative_ads.push_back(creative_ad_2);

  database::SaveCreativeNotificationAds(creative_ads);

  // Act
  const ScopedPacingRandomNumberSetterForTesting scoped_setter(0.3);

  CreativeNotificationAdList expected_creative_ads = {creative_ad_2};

  eligible_ads_->GetForUserModel(
      BuildUserModelForTesting(
          /*intent_segments*/ {},
          /*latent_interest_segments*/ {},
          /*interest_segments*/ {"technology & computing", "food & drink"},
          /*text_embedding_html_events*/ {}),
      base::BindOnce(
          [](const CreativeNotificationAdList& expected_creative_ads,
             const bool had_opportunity,
             const CreativeNotificationAdList& creative_ads) {
            // Assert
            EXPECT_TRUE(had_opportunity);
            EXPECT_EQ(expected_creative_ads, creative_ads);
          },
          std::move(expected_creative_ads)));
}

TEST_F(HnsAdsEligibleNotificationAdsV1Test, GetPrioritizedAds) {
  // Arrange
  CreativeNotificationAdList creative_ads;

  CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_1.segment = "technology & computing";
  creative_ad_1.priority = 1;
  creative_ads.push_back(creative_ad_1);

  CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_2.segment = "finance-banking";
  creative_ad_2.priority = 1;
  creative_ads.push_back(creative_ad_2);

  CreativeNotificationAdInfo creative_ad_3 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_3.segment = "food & drink";
  creative_ad_3.priority = 2;
  creative_ads.push_back(creative_ad_3);

  database::SaveCreativeNotificationAds(creative_ads);

  // Act
  CreativeNotificationAdList expected_creative_ads = {creative_ad_1};

  eligible_ads_->GetForUserModel(
      BuildUserModelForTesting(
          /*intent_segments*/ {},
          /*latent_interest_segments*/ {},
          /*interest_segments*/ {"technology & computing", "food & drink"},
          /*text_embedding_html_events*/ {}),
      base::BindOnce(
          [](const CreativeNotificationAdList& expected_creative_ads,
             const bool had_opportunity,
             const CreativeNotificationAdList& creative_ads) {
            // Assert
            EXPECT_TRUE(had_opportunity);
            EXPECT_EQ(expected_creative_ads, creative_ads);
          },
          std::move(expected_creative_ads)));
}

}  // namespace hns_ads
