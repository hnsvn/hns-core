/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_builder_util.h"

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsConversionQueueItemBuilderUtilTest : public UnitTestBase {};

TEST_F(HnsAdsConversionQueueItemBuilderUtilTest, ProcessQueueItemAt) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ProcessQueueItemAt().is_null());
}

}  // namespace hns_ads
