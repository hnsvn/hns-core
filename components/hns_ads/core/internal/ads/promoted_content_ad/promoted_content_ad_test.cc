/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/test/mock_callback.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/history/history_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/permission_rules/permission_rules_unittest_util.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"
#include "net/http/http_status_code.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsPromotedContentAdIntegrationTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUpForTesting(/*is_integration_test*/ true);

    ForcePermissionRulesForTesting();
  }

  void SetUpMocks() override {
    const URLResponseMap url_responses = {
        {BuildCatalogUrlPath(),
         {{net::HTTP_OK,
           /*response_body*/ "/catalog_with_promoted_content_ad.json"}}}};
    MockUrlResponses(ads_client_mock_, url_responses);

    EXPECT_CALL(ads_client_mock_, RecordP2AEvents).Times(0);
  }

  void TriggerPromotedContentAdEvent(
      const std::string& placement_id,
      const std::string& creative_instance_id,
      const mojom::PromotedContentAdEventType& event_type,
      const bool should_fire_event) {
    base::MockCallback<TriggerAdEventCallback> callback;
    EXPECT_CALL(callback, Run(/*success*/ should_fire_event));

    GetAds().TriggerPromotedContentAdEvent(placement_id, creative_instance_id,
                                           event_type, callback.Get());
  }
};

TEST_F(HnsAdsPromotedContentAdIntegrationTest, TriggerViewedEvent) {
  // Arrange

  // Act
  TriggerPromotedContentAdEvent(kPlacementId, kCreativeInstanceId,
                                mojom::PromotedContentAdEventType::kViewed,
                                /*should_fire_event*/ true);

  // Assert
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kPromotedContentAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kPromotedContentAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(1U, GetHistoryItemCountForTesting());
  EXPECT_EQ(1U, GetTransactionCountForTesting());
}

TEST_F(HnsAdsPromotedContentAdIntegrationTest, TriggerClickedEvent) {
  // Arrange
  TriggerPromotedContentAdEvent(kPlacementId, kCreativeInstanceId,
                                mojom::PromotedContentAdEventType::kViewed,
                                /*should_fire_event*/ true);

  // Act
  TriggerPromotedContentAdEvent(kPlacementId, kCreativeInstanceId,
                                mojom::PromotedContentAdEventType::kClicked,
                                /*should_fire_event*/ true);

  // Assert
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kPromotedContentAd,
                                          ConfirmationType::kServed));
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kPromotedContentAd,
                                          ConfirmationType::kViewed));
  EXPECT_EQ(1U, GetAdEventCountForTesting(AdType::kPromotedContentAd,
                                          ConfirmationType::kClicked));
  EXPECT_EQ(2U, GetHistoryItemCountForTesting());
  EXPECT_EQ(2U, GetTransactionCountForTesting());
}

}  // namespace hns_ads
