/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/test/mock_callback.h"
#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/history/history_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rules_unittest_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "hns/components/hns_ads/core/public/ads/new_tab_page_ad_info.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"
#include "hns/components/hns_ads/core/public/feature/hns_ads_feature.h"
#include "net/http/http_status_code.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsNewTabPageAdIntegrationTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUpForTesting(/*is_integration_test*/ true);
  }

  void SetUpMocks() override {
    const URLResponseMap url_responses = {
        {BuildCatalogUrlPath(),
         {{net::HTTP_OK,
           /*response_body*/ "/catalog_with_new_tab_page_ad.json"}}}};
    MockUrlResponses(ads_client_mock_, url_responses);

    EXPECT_CALL(ads_client_mock_, RecordP2AEvents).Times(0);
  }

  void TriggerNewTabPageAdEvent(const std::string& placement_id,
                                const std::string& creative_instance_id,
                                const mojom::NewTabPageAdEventType& event_type,
                                const bool should_fire_event) {
    base::MockCallback<TriggerAdEventCallback> callback;
    EXPECT_CALL(callback, Run(/*success*/ should_fire_event));

    GetAds().TriggerNewTabPageAdEvent(placement_id, creative_instance_id,
                                      event_type, callback.Get());
  }

  void TriggerNewTabPageAdEvents(
      const std::string& placement_id,
      const std::string& creative_instance_id,
      const std::vector<mojom::NewTabPageAdEventType>& event_types,
      const bool should_fire_event) {
    for (const auto& event_type : event_types) {
      TriggerNewTabPageAdEvent(placement_id, creative_instance_id, event_type,
                               should_fire_event);
    }
  }
};

TEST_F(HnsAdsNewTabPageAdIntegrationTest, Serve) {
  // Arrange
  ForcePermissionRulesForTesting();

  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsNewTabPageAdEventsFeature, params);

  std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  base::MockCallback<MaybeServeNewTabPageAdCallback> callback;
  EXPECT_CALL(callback, Run)
      .WillOnce([](const absl::optional<NewTabPageAdInfo>& ad) {
        // Assert
        EXPECT_TRUE(ad);
        EXPECT_TRUE(ad->IsValid());
        EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                                ConfirmationType::kServed));
      });

  // Act
  GetAds().MaybeServeNewTabPageAd(callback.Get());
}

TEST_F(HnsAdsNewTabPageAdIntegrationTest, DoNotServe) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsNewTabPageAdEventsFeature, params);

  std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  absl::optional<NewTabPageAdInfo> ad;
  base::MockCallback<MaybeServeNewTabPageAdCallback> callback;
  EXPECT_CALL(callback, Run(ad));

  // Act
  GetAds().MaybeServeNewTabPageAd(callback.Get());
}

TEST_F(HnsAdsNewTabPageAdIntegrationTest, TriggerViewedEvent) {
  // Arrange
  ForcePermissionRulesForTesting();

  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsNewTabPageAdEventsFeature, params);

  std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  base::MockCallback<MaybeServeNewTabPageAdCallback> callback;
  EXPECT_CALL(callback, Run)
      .WillOnce([=](const absl::optional<NewTabPageAdInfo>& ad) {
        // Assert
        ASSERT_TRUE(ad);
        ASSERT_TRUE(ad->IsValid());
        ASSERT_EQ(1U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                                ConfirmationType::kServed));

        // Act
        TriggerNewTabPageAdEvent(ad->placement_id, ad->creative_instance_id,
                                 mojom::NewTabPageAdEventType::kViewed,
                                 /*should_fire_event*/ true);

        // Assert
        EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                                ConfirmationType::kViewed));
        EXPECT_EQ(1U, GetHistoryItemCountForTesting());
        EXPECT_EQ(1U, GetTransactionCountForTesting());
      });

  GetAds().MaybeServeNewTabPageAd(callback.Get());
}

TEST_F(HnsAdsNewTabPageAdIntegrationTest,
       TriggerViewedEventForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsNewTabPageAdEventsFeature, params);

  std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act
  TriggerNewTabPageAdEvent(kPlacementId, kCreativeInstanceId,
                           mojom::NewTabPageAdEventType::kViewed,
                           /*should_fire_event*/ true);

  // Assert
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(0U, GetHistoryItemCountForTesting());
  EXPECT_EQ(0U, GetTransactionCountForTesting());
}

TEST_F(
    HnsAdsNewTabPageAdIntegrationTest,
    DoNotTriggerViewedEventIfShouldNotAlwaysTriggerAdEventsAndRewardsAreDisabled) {
  // Arrange
  DisableHnsRewardsForTesting();

  // Act
  TriggerNewTabPageAdEvent(kPlacementId, kCreativeInstanceId,
                           mojom::NewTabPageAdEventType::kViewed,
                           /*should_fire_event*/ false);

  // Assert
  EXPECT_EQ(0U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(0U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(0U, GetHistoryItemCountForTesting());
  EXPECT_EQ(0U, GetTransactionCountForTesting());
}

TEST_F(HnsAdsNewTabPageAdIntegrationTest, TriggerClickedEvent) {
  // Arrange
  ForcePermissionRulesForTesting();

  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsNewTabPageAdEventsFeature, params);

  std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  base::MockCallback<MaybeServeNewTabPageAdCallback> callback;
  EXPECT_CALL(callback, Run)
      .WillOnce([=](const absl::optional<NewTabPageAdInfo>& ad) {
        // Assert
        ASSERT_TRUE(ad);
        ASSERT_TRUE(ad->IsValid());
        ASSERT_EQ(1U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                                ConfirmationType::kServed));

        TriggerNewTabPageAdEvent(ad->placement_id, ad->creative_instance_id,
                                 mojom::NewTabPageAdEventType::kViewed,
                                 /*should_fire_event*/ true);

        // Act
        TriggerNewTabPageAdEvent(ad->placement_id, ad->creative_instance_id,
                                 mojom::NewTabPageAdEventType::kClicked,
                                 /*should_fire_event*/ true);

        // Assert
        EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                                ConfirmationType::kViewed));
        EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                                ConfirmationType::kClicked));
        EXPECT_EQ(2U, GetHistoryItemCountForTesting());
        EXPECT_EQ(2U, GetTransactionCountForTesting());
      });

  GetAds().MaybeServeNewTabPageAd(callback.Get());
}

TEST_F(HnsAdsNewTabPageAdIntegrationTest,
       TriggerClickedEventForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsNewTabPageAdEventsFeature, params);

  std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  TriggerNewTabPageAdEvent(kPlacementId, kCreativeInstanceId,
                           mojom::NewTabPageAdEventType::kViewed,
                           /*should_fire_event*/ true);

  // Act
  TriggerNewTabPageAdEvent(kPlacementId, kCreativeInstanceId,
                           mojom::NewTabPageAdEventType::kClicked,
                           /*should_fire_event*/ true);

  // Assert
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                          ConfirmationType::kClicked));
  EXPECT_EQ(0U, GetHistoryItemCountForTesting());
  EXPECT_EQ(0U, GetTransactionCountForTesting());
}

TEST_F(
    HnsAdsNewTabPageAdIntegrationTest,
    DoNotTriggerClickedEventIfShouldNotAlwaysTriggerAdEventsAndHnsRewardsAreDisabled) {
  // Arrange
  DisableHnsRewardsForTesting();

  TriggerNewTabPageAdEvent(kPlacementId, kCreativeInstanceId,
                           mojom::NewTabPageAdEventType::kViewed,
                           /*should_fire_event*/ false);

  // Act
  TriggerNewTabPageAdEvent(kPlacementId, kCreativeInstanceId,
                           mojom::NewTabPageAdEventType::kClicked,
                           /*should_fire_event*/ false);

  // Assert
  EXPECT_EQ(0U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(0U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(0U, GetAdEventCountForTesting(AdType::kNewTabPageAd,
                                          ConfirmationType::kClicked));
  EXPECT_EQ(0U, GetHistoryItemCountForTesting());
  EXPECT_EQ(0U, GetTransactionCountForTesting());
}

}  // namespace hns_ads
