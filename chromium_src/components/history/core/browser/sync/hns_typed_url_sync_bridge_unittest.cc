/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

// Run all TypedURLSyncBridgeTest again but with kHnsSyncSendAllHistory
// feature enabled

#include "hns/components/history/core/browser/sync/hns_typed_url_sync_bridge.h"
#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_sync/features.h"

#define HNS_TEST_MEMBERS_DECLARE                    \
  base::test::ScopedFeatureList scoped_feature_list_; \
  int kVisitThrottleThreshold;                        \
  int kVisitThrottleMultiple;

#define HNS_TEST_MEMBERS_INIT                                       \
  scoped_feature_list_.InitWithFeatures(                              \
      {hns_sync::features::kHnsSyncSendAllHistory}, {});          \
  /* Need these overrides only for                               */   \
  /* HnsTypedURLSyncBridgeTest.ThrottleVisitLocalTypedUrl only */   \
  kVisitThrottleThreshold =                                           \
      typed_url_sync_bridge_->GetSendAllFlagVisitThrottleThreshold(); \
  kVisitThrottleMultiple =                                            \
      typed_url_sync_bridge_->GetSendAllFlagVisitThrottleMultiple();

#define TypedURLSyncBridge HnsTypedURLSyncBridge
#define TypedURLSyncBridgeTest HnsTypedURLSyncBridgeTest
#include "src/components/history/core/browser/sync/typed_url_sync_bridge_unittest.cc"
#undef TypedURLSyncBridgeTest
#undef TypedURLSyncBridge
#undef HNS_TEST_MEMBERS_INIT
#undef HNS_TEST_MEMBERS_DECLARE

namespace {

bool IsSendAllHistoryEnabled() {
  return base::FeatureList::IsEnabled(
      hns_sync::features::kHnsSyncSendAllHistory);
}

}  // namespace

namespace history {

URLRow MakeUrlRow(int visit_count, int typed_count) {
  URLRow urlRow;
  urlRow.set_visit_count(visit_count);
  urlRow.set_typed_count(typed_count);
  return urlRow;
}

TEST_F(HnsTypedURLSyncBridgeTest, HnsShouldSyncVisit) {
  ASSERT_TRUE(IsSendAllHistoryEnabled());

  EXPECT_TRUE(
      bridge()->ShouldSyncVisit(MakeUrlRow(1, 0), ui::PAGE_TRANSITION_LINK));
  EXPECT_TRUE(
      bridge()->ShouldSyncVisit(MakeUrlRow(1, 0), ui::PAGE_TRANSITION_TYPED));
  EXPECT_TRUE(
      bridge()->ShouldSyncVisit(MakeUrlRow(20, 0), ui::PAGE_TRANSITION_LINK));
  EXPECT_FALSE(
      bridge()->ShouldSyncVisit(MakeUrlRow(21, 0), ui::PAGE_TRANSITION_LINK));
  EXPECT_TRUE(
      bridge()->ShouldSyncVisit(MakeUrlRow(30, 0), ui::PAGE_TRANSITION_LINK));

  {
    base::test::ScopedFeatureList scoped_feature_list2;
    scoped_feature_list2.InitWithFeatures(
        {}, {hns_sync::features::kHnsSyncSendAllHistory});
    EXPECT_FALSE(
        bridge()->ShouldSyncVisit(MakeUrlRow(1, 0), ui::PAGE_TRANSITION_LINK));
    EXPECT_TRUE(
        bridge()->ShouldSyncVisit(MakeUrlRow(1, 1), ui::PAGE_TRANSITION_TYPED));
    EXPECT_FALSE(bridge()->ShouldSyncVisit(MakeUrlRow(20, 20),
                                           ui::PAGE_TRANSITION_LINK));
    EXPECT_TRUE(bridge()->ShouldSyncVisit(MakeUrlRow(20, 20),
                                          ui::PAGE_TRANSITION_TYPED));
  }
}

}  // namespace history
