/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/conversion_user_data_util.h"

#include <string>

#include "base/json/json_writer.h"
#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_builder.h"
#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_builder.h"
#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_info.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_info.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_builder.h"
#include "hns/components/hns_ads/core/public/ad_info.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/re2/src/re2/re2.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsConversionUserDataUtilTest : public UnitTestBase {};

TEST_F(HnsAdsConversionUserDataUtilTest, BuildVerifiableConversionUserData) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ false);
  const ConversionInfo conversion = BuildConversion(
      BuildAdEvent(ad, ConfirmationType::kViewed,
                   /*created_at*/ Now()),
      VerifiableConversionInfo{kVerifiableConversionId,
                               kVerifiableConversionAdvertiserPublicKey});
  const ConversionQueueItemInfo conversion_queue_item =
      BuildConversionQueueItem(conversion,
                               /*process_at*/ Now());

  // Act
  const absl::optional<base::Value::Dict> user_data =
      MaybeBuildVerifiableConversionUserData(conversion_queue_item);
  ASSERT_TRUE(user_data);

  // Assert
  std::string json;
  ASSERT_TRUE(base::JSONWriter::Write(*user_data, &json));

  const std::string pattern =
      R"({"envelope":{"alg":"crypto_box_curve25519xsalsa20poly1305","ciphertext":".{64}","epk":".{44}","nonce":".{32}"}})";
  EXPECT_TRUE(RE2::FullMatch(json, pattern));
}

TEST_F(HnsAdsConversionUserDataUtilTest,
       DoNotBuildVerifiableConversionUserData) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ false);
  const ConversionInfo conversion =
      BuildConversion(BuildAdEvent(ad, ConfirmationType::kViewed,
                                   /*created_at*/ Now()),
                      /*verifiable_conversion*/ absl::nullopt);
  const ConversionQueueItemInfo conversion_queue_item =
      BuildConversionQueueItem(conversion,
                               /*process_at*/ Now());

  // Act

  // Assert
  EXPECT_FALSE(MaybeBuildVerifiableConversionUserData(conversion_queue_item));
}

TEST_F(HnsAdsConversionUserDataUtilTest, BuildConversionActionTypeUserData) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ false);
  const ConversionInfo conversion =
      BuildConversion(BuildAdEvent(ad, ConfirmationType::kViewed,
                                   /*created_at*/ Now()),
                      /*verifiable_conversion*/ absl::nullopt);
  const ConversionQueueItemInfo conversion_queue_item =
      BuildConversionQueueItem(conversion,
                               /*process_at*/ Now());

  // Act

  // Assert
  EXPECT_EQ(base::test::ParseJsonDict(R"({"action":"view"})"),
            BuildConversionActionTypeUserData(conversion_queue_item));
}

}  // namespace hns_ads
