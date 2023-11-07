/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/ads/search_result_ad/search_result_ad_handler.h"
#include "hns/components/hns_ads/core/public/feature/hns_ads_feature.h"

#include "base/test/mock_callback.h"
#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/search_result_ads/search_result_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/history/history_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rules_unittest_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsSearchResultAdIntegrationTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUpForTesting(/*is_integration_test*/ true);

    ForcePermissionRulesForTesting();
  }

  void SetUpMocks() override {
    EXPECT_CALL(ads_client_mock_, RecordP2AEvents).Times(0);
  }

  void TriggerSearchResultAdEvent(
      mojom::SearchResultAdInfoPtr ad_mojom,
      const mojom::SearchResultAdEventType& event_type,
      const bool should_fire_event) {
    base::MockCallback<TriggerAdEventCallback> callback;
    EXPECT_CALL(callback, Run(/*success*/ should_fire_event));

    GetAds().TriggerSearchResultAdEvent(std::move(ad_mojom), event_type,
                                        callback.Get());
  }

  void TriggerSearchResultAdEvents(
      mojom::SearchResultAdInfoPtr ad_mojom,
      const std::vector<mojom::SearchResultAdEventType>& event_types,
      const bool should_fire_event) {
    for (const auto& event_type : event_types) {
      TriggerSearchResultAdEvent(ad_mojom->Clone(), event_type,
                                 should_fire_event);
    }
  }
};

TEST_F(HnsAdsSearchResultAdIntegrationTest, TriggerViewedEvents) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsSearchResultAdEventsFeature, params);

  std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act
  TriggerSearchResultAdEvent(
      BuildSearchResultAdForTesting(/*should_use_random_uuids*/ true),
      mojom::SearchResultAdEventType::kViewed,
      /*should_fire*/ true);

  TriggerSearchResultAdEvent(
      BuildSearchResultAdForTesting(/*should_use_random_uuids*/ true),
      mojom::SearchResultAdEventType::kViewed,
      /*should_fire*/ true);

  // Assert
  EXPECT_EQ(2U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(2U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(2U, GetHistoryItemCountForTesting());
  EXPECT_EQ(2U, GetTransactionCountForTesting());
}

TEST_F(HnsAdsSearchResultAdIntegrationTest, TriggerQueuedViewedEvents) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsSearchResultAdEventsFeature, params);

  std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  SearchResultAd::DeferTriggeringOfAdViewedEvent();

  // Act
  TriggerSearchResultAdEvent(
      // This ad viewed event triggering will be deferred.
      BuildSearchResultAdForTesting(/*should_use_random_uuids*/ true),
      mojom::SearchResultAdEventType::kViewed,
      /*should_fire*/ true);

  TriggerSearchResultAdEvent(
      // This ad viewed event will be queued as the previous ad viewed event has
      // not completed.
      BuildSearchResultAdForTesting(/*should_use_random_uuids*/ true),
      mojom::SearchResultAdEventType::kViewed,
      /*should_fire*/ true);

  ASSERT_EQ(2U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kServed));
  ASSERT_EQ(1U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kViewed));
  ASSERT_EQ(1U, GetHistoryItemCountForTesting());
  ASSERT_EQ(1U, GetTransactionCountForTesting());

  SearchResultAd::TriggerDeferredAdViewedEvent();

  // Assert
  EXPECT_EQ(2U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(2U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(2U, GetHistoryItemCountForTesting());
  EXPECT_EQ(2U, GetTransactionCountForTesting());
}

TEST_F(HnsAdsSearchResultAdIntegrationTest, TriggerClickedEvent) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsSearchResultAdEventsFeature, params);

  std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  const mojom::SearchResultAdInfoPtr search_result_ad =
      BuildSearchResultAdForTesting(/*should_use_random_uuids*/ true);

  TriggerSearchResultAdEvent(search_result_ad.Clone(),
                             mojom::SearchResultAdEventType::kViewed,
                             /*should_fire*/ true);

  // Act
  TriggerSearchResultAdEvent(search_result_ad.Clone(),
                             mojom::SearchResultAdEventType::kClicked,
                             /*should_fire*/ true);

  // Assert
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kClicked));
  EXPECT_EQ(2U, GetHistoryItemCountForTesting());
  EXPECT_EQ(2U, GetTransactionCountForTesting());
}

TEST_F(HnsAdsSearchResultAdIntegrationTest,
       TriggerViewedEventsForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsSearchResultAdEventsFeature, params);

  std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  // Act
  TriggerSearchResultAdEvent(
      BuildSearchResultAdForTesting(/*should_use_random_uuids*/ true),
      mojom::SearchResultAdEventType::kViewed,
      /*should_fire*/ true);

  TriggerSearchResultAdEvent(
      BuildSearchResultAdForTesting(/*should_use_random_uuids*/ true),
      mojom::SearchResultAdEventType::kViewed,
      /*should_fire*/ true);

  // Assert
  EXPECT_EQ(2U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(2U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(0U, GetHistoryItemCountForTesting());
  EXPECT_EQ(0U, GetTransactionCountForTesting());
}

TEST_F(
    HnsAdsSearchResultAdIntegrationTest,
    DoNotTriggerViewedEventIfShouldNotAlwaysTriggerAdEventsAndHnsRewardsAreDisabled) {
  // Arrange
  DisableHnsRewardsForTesting();

  // Act
  TriggerSearchResultAdEvent(
      BuildSearchResultAdForTesting(/*should_use_random_uuids*/ true),
      mojom::SearchResultAdEventType::kViewed,
      /*should_fire*/ false);

  // Assert
  EXPECT_EQ(0U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(0U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(0U, GetHistoryItemCountForTesting());
  EXPECT_EQ(0U, GetTransactionCountForTesting());
}

TEST_F(HnsAdsSearchResultAdIntegrationTest,
       TriggerQueuedViewedEventsForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsSearchResultAdEventsFeature, params);

  std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  SearchResultAd::DeferTriggeringOfAdViewedEvent();

  // Act
  TriggerSearchResultAdEvent(
      // This ad viewed event triggering will be deferred.
      BuildSearchResultAdForTesting(/*should_use_random_uuids*/ true),
      mojom::SearchResultAdEventType::kViewed,
      /*should_fire*/ true);

  TriggerSearchResultAdEvent(
      // This ad viewed event will be queued as the previous ad viewed event has
      // not completed.
      BuildSearchResultAdForTesting(/*should_use_random_uuids*/ true),
      mojom::SearchResultAdEventType::kViewed,
      /*should_fire*/ true);

  ASSERT_EQ(2U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kServed));
  ASSERT_EQ(1U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kViewed));
  ASSERT_EQ(0U, GetHistoryItemCountForTesting());
  ASSERT_EQ(0U, GetTransactionCountForTesting());

  SearchResultAd::TriggerDeferredAdViewedEvent();

  // Assert
  EXPECT_EQ(2U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(2U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(0U, GetHistoryItemCountForTesting());
  EXPECT_EQ(0U, GetTransactionCountForTesting());
}

TEST_F(HnsAdsSearchResultAdIntegrationTest,
       TriggerClickedEventForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  enabled_features.emplace_back(
      kShouldAlwaysTriggerHnsSearchResultAdEventsFeature, params);

  std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  const mojom::SearchResultAdInfoPtr search_result_ad =
      BuildSearchResultAdForTesting(/*should_use_random_uuids*/ true);

  TriggerSearchResultAdEvent(search_result_ad.Clone(),
                             mojom::SearchResultAdEventType::kViewed,
                             /*should_fire*/ true);

  // Act
  TriggerSearchResultAdEvent(search_result_ad.Clone(),
                             mojom::SearchResultAdEventType::kClicked,
                             /*should_fire*/ true);

  // Assert
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kClicked));
  EXPECT_EQ(0U, GetHistoryItemCountForTesting());
  EXPECT_EQ(0U, GetTransactionCountForTesting());
}

TEST_F(
    HnsAdsSearchResultAdIntegrationTest,
    DoNotTriggerClickedEventIfShouldNotAlwaysTriggerAdEventsAndHnsRewardsAreDisabled) {
  // Arrange
  DisableHnsRewardsForTesting();

  const mojom::SearchResultAdInfoPtr search_result_ad =
      BuildSearchResultAdForTesting(/*should_use_random_uuids*/ true);

  TriggerSearchResultAdEvent(search_result_ad.Clone(),
                             mojom::SearchResultAdEventType::kViewed,
                             /*should_fire*/ false);

  // Act
  TriggerSearchResultAdEvent(search_result_ad.Clone(),
                             mojom::SearchResultAdEventType::kClicked,
                             /*should_fire*/ false);

  // Assert
  EXPECT_EQ(0U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(0U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(0U, GetAdEventCountForTesting(AdType::kSearchResultAd,
                                          ConfirmationType::kClicked));
  EXPECT_EQ(0U, GetHistoryItemCountForTesting());
  EXPECT_EQ(0U, GetTransactionCountForTesting());
}

}  // namespace hns_ads
