/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/purchase_intent_processor.h"

#include <cstdint>
#include <memory>

#include "base/ranges/algorithm.h"
#include "hns/components/hns_ads/core/internal/common/resources/country_components_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/client_state_manager.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/model/purchase_intent_model.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/resource/purchase_intent_resource.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/resource/purchase_intent_signal_history_info.h"
#include "url/gurl.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsPurchaseIntentProcessorTest : public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    resource_ = std::make_unique<PurchaseIntentResource>();
  }

  bool LoadResource() {
    NotifyDidUpdateResourceComponent(kCountryComponentManifestVersion,
                                     kCountryComponentId);
    task_environment_.RunUntilIdle();
    return resource_->IsInitialized();
  }

  std::unique_ptr<PurchaseIntentResource> resource_;
};

TEST_F(HnsAdsPurchaseIntentProcessorTest,
       DoNotProcessIfResourceIsNotInitialized) {
  // Arrange

  // Act
  const GURL url = GURL("https://www.hns.com/test?foo=bar");
  PurchaseIntentProcessor processor(*resource_);
  processor.Process(url);

  // Assert
  const PurchaseIntentSignalHistoryMap& history =
      ClientStateManager::GetInstance().GetPurchaseIntentSignalHistory();

  EXPECT_TRUE(history.empty());
}

TEST_F(HnsAdsPurchaseIntentProcessorTest, DoNotProcessForInvalidUrl) {
  // Arrange
  ASSERT_TRUE(LoadResource());

  // Act
  const GURL url = GURL("INVALID");
  PurchaseIntentProcessor processor(*resource_);
  processor.Process(url);

  // Assert
  const PurchaseIntentSignalHistoryMap& history =
      ClientStateManager::GetInstance().GetPurchaseIntentSignalHistory();

  EXPECT_TRUE(history.empty());
}

TEST_F(HnsAdsPurchaseIntentProcessorTest, NeverProcessed) {
  // Arrange
  ASSERT_TRUE(LoadResource());

  const PurchaseIntentModel model;

  // Act
  const SegmentList segments = model.GetSegments();

  // Assert
  const PurchaseIntentSignalHistoryMap history =
      ClientStateManager::GetInstance().GetPurchaseIntentSignalHistory();

  EXPECT_TRUE(history.empty());
}

TEST_F(HnsAdsPurchaseIntentProcessorTest, ProcessUrl) {
  // Arrange
  ASSERT_TRUE(LoadResource());

  // Act
  const GURL url = GURL("https://www.hns.com/test?foo=bar");
  PurchaseIntentProcessor processor(*resource_);
  processor.Process(url);

  // Assert
  const PurchaseIntentSignalHistoryMap& history =
      ClientStateManager::GetInstance().GetPurchaseIntentSignalHistory();

  const base::Time now = Now();
  const uint16_t weight = 1;

  const PurchaseIntentSignalHistoryMap expected_history = {
      {"segment 2", {PurchaseIntentSignalHistoryInfo(now, weight)}},
      {"segment 3", {PurchaseIntentSignalHistoryInfo(now, weight)}}};

  EXPECT_TRUE(base::ranges::equal(expected_history, history));
}

TEST_F(HnsAdsPurchaseIntentProcessorTest, ProcessMultipleMatchingUrls) {
  // Arrange
  ASSERT_TRUE(LoadResource());

  // Act
  PurchaseIntentProcessor processor(*resource_);

  const GURL url = GURL("https://www.hns.com/test?foo=bar");
  processor.Process(url);
  processor.Process(url);

  // Assert
  const PurchaseIntentSignalHistoryMap& history =
      ClientStateManager::GetInstance().GetPurchaseIntentSignalHistory();

  const base::Time now = Now();
  const uint16_t weight = 1;

  const PurchaseIntentSignalHistoryMap expected_history = {
      {"segment 2",
       {PurchaseIntentSignalHistoryInfo(now, weight),
        PurchaseIntentSignalHistoryInfo(now, weight)}},
      {"segment 3",
       {PurchaseIntentSignalHistoryInfo(now, weight),
        PurchaseIntentSignalHistoryInfo(now, weight)}}};

  EXPECT_TRUE(base::ranges::equal(expected_history, history));
}

TEST_F(HnsAdsPurchaseIntentProcessorTest, ProcessMultipleUniqueUrls) {
  // Arrange
  ASSERT_TRUE(LoadResource());

  // Act
  PurchaseIntentProcessor processor(*resource_);

  const base::Time now_1 = Now();
  const GURL url_1 = GURL("https://www.hns.com/test?foo=bar");
  processor.Process(url_1);

  AdvanceClockBy(base::Minutes(5));

  const base::Time now_2 = Now();
  const GURL url_2 = GURL("https://www.basicattentiontoken.org/test?foo=bar");
  processor.Process(url_2);

  // Assert
  const PurchaseIntentSignalHistoryMap& history =
      ClientStateManager::GetInstance().GetPurchaseIntentSignalHistory();

  const uint16_t weight = 1;

  const PurchaseIntentSignalHistoryMap expected_history = {
      {"segment 2",
       {PurchaseIntentSignalHistoryInfo(now_1, weight),
        PurchaseIntentSignalHistoryInfo(now_2, weight)}},
      {"segment 3",
       {PurchaseIntentSignalHistoryInfo(now_1, weight),
        PurchaseIntentSignalHistoryInfo(now_2, weight)}}};

  EXPECT_TRUE(base::ranges::equal(expected_history, history));
}

TEST_F(HnsAdsPurchaseIntentProcessorTest, ProcessMultipleMatchingKeywords) {
  // Arrange
  ASSERT_TRUE(LoadResource());

  // Act
  PurchaseIntentProcessor processor(*resource_);

  const base::Time now_1 = Now();
  const GURL url_1 =
      GURL("https://duckduckgo.com/?q=segment+keyword+1&foo=bar");
  processor.Process(url_1);

  AdvanceClockBy(base::Minutes(5));

  const base::Time now_2 = Now();
  const GURL url_2 =
      GURL("https://duckduckgo.com/?q=segment+keyword+2&bar=foo");
  processor.Process(url_2);

  // Assert
  const PurchaseIntentSignalHistoryMap& history =
      ClientStateManager::GetInstance().GetPurchaseIntentSignalHistory();

  const uint16_t weight = 1;

  const PurchaseIntentSignalHistoryMap expected_history = {
      {"segment 1",
       {PurchaseIntentSignalHistoryInfo(now_1, weight),
        PurchaseIntentSignalHistoryInfo(now_2, weight)}},
      {"segment 2", {PurchaseIntentSignalHistoryInfo(now_2, weight)}}};

  EXPECT_TRUE(base::ranges::equal(expected_history, history));
}

TEST_F(HnsAdsPurchaseIntentProcessorTest, ProcessMultipleUniqueKeywords) {
  // Arrange
  ASSERT_TRUE(LoadResource());

  // Act
  PurchaseIntentProcessor processor(*resource_);

  const base::Time now_1 = Now();
  const GURL url_1 =
      GURL("https://duckduckgo.com/?q=segment+keyword+1&foo=bar");
  processor.Process(url_1);

  AdvanceClockBy(base::Minutes(5));

  const base::Time now_2 = Now();
  const GURL url_2 =
      GURL("https://www.google.com/search?q=segment+keyword+1&bar=foo");
  processor.Process(url_2);

  // Assert
  const PurchaseIntentSignalHistoryMap& history =
      ClientStateManager::GetInstance().GetPurchaseIntentSignalHistory();

  const uint16_t weight = 1;

  const PurchaseIntentSignalHistoryMap expected_history = {
      {"segment 1",
       {PurchaseIntentSignalHistoryInfo(now_1, weight),
        PurchaseIntentSignalHistoryInfo(now_2, weight)}}};

  EXPECT_EQ(expected_history, history);
}

TEST_F(HnsAdsPurchaseIntentProcessorTest, ProcessSegmentAndFunnelKeywords) {
  // Arrange
  ASSERT_TRUE(LoadResource());

  // Act
  PurchaseIntentProcessor processor(*resource_);

  const GURL url =
      GURL("https://duckduckgo.com/?q=segment+keyword+1+funnel+keyword+2");
  processor.Process(url);

  // Assert
  const PurchaseIntentSignalHistoryMap& history =
      ClientStateManager::GetInstance().GetPurchaseIntentSignalHistory();

  const PurchaseIntentSignalHistoryMap expected_history = {
      {"segment 1",
       {
           PurchaseIntentSignalHistoryInfo(Now(), /*weight*/ 3),
       }}};

  EXPECT_EQ(expected_history, history);
}

}  // namespace hns_ads
