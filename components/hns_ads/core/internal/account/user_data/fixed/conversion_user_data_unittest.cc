/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/conversion_user_data.h"

#include "base/json/json_writer.h"
#include "base/test/mock_callback.h"
#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/account/user_data/build_user_data_callback.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_unittest_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "third_party/re2/src/re2/re2.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsConversionUserDataBuilderTest : public UnitTestBase {};

TEST_F(HnsAdsConversionUserDataBuilderTest,
       BuildConversionUserDataForRewardsUser) {
  // Arrange
  BuildAndSaveConversionQueueItemsForTesting(
      AdType::kNotificationAd, ConfirmationType::kViewed,
      /*is_verifiable*/ false, /*should_use_random_uuids*/ false, /*count*/ 1);

  // Assert
  const base::Value::Dict expected_user_data =
      base::test::ParseJsonDict(R"({"conversion":[{"action":"view"}]})");
  base::MockCallback<BuildUserDataCallback> callback;
  EXPECT_CALL(callback, Run(::testing::Eq(std::ref(expected_user_data))));

  // Act
  BuildConversionUserData(kCreativeInstanceId, callback.Get());
}

TEST_F(HnsAdsConversionUserDataBuilderTest,
       BuildVerifiableConversionUserDataForRewardsUser) {
  // Arrange
  BuildAndSaveConversionQueueItemsForTesting(
      AdType::kNotificationAd, ConfirmationType::kClicked,
      /*is_verifiable*/ true, /*should_use_random_uuids*/ false, /*count*/ 1);

  // Assert
  base::MockCallback<BuildUserDataCallback> callback;
  EXPECT_CALL(callback, Run).WillOnce([](base::Value::Dict user_data) {
    std::string json;
    ASSERT_TRUE(base::JSONWriter::Write(user_data, &json));
    const std::string pattern =
        R"({"conversion":\[{"action":"click"},{"envelope":{"alg":"crypto_box_curve25519xsalsa20poly1305","ciphertext":".{64}","epk":".{44}","nonce":".{32}"}}]})";
    EXPECT_TRUE(RE2::FullMatch(json, pattern));
  });

  // Act
  BuildConversionUserData(kCreativeInstanceId, callback.Get());
}

TEST_F(HnsAdsConversionUserDataBuilderTest,
       BuildConversionUserDataForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  BuildAndSaveConversionQueueItemsForTesting(
      AdType::kNotificationAd, ConfirmationType::kViewed,
      /*is_verifiable*/ false, /*should_use_random_uuids*/ false, /*count*/ 1);

  // Assert
  const base::Value::Dict expected_user_data =
      base::test::ParseJsonDict(R"({"conversion":[{"action":"view"}]})");
  base::MockCallback<BuildUserDataCallback> callback;
  EXPECT_CALL(callback, Run(::testing::Eq(std::ref(expected_user_data))));

  // Act
  BuildConversionUserData(kCreativeInstanceId, callback.Get());
}

TEST_F(HnsAdsConversionUserDataBuilderTest,
       BuildVerifiableConversionUserDataForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  BuildAndSaveConversionQueueItemsForTesting(
      AdType::kNotificationAd, ConfirmationType::kClicked,
      /*is_verifiable*/ true, /*should_use_random_uuids*/ false, /*count*/ 1);

  // Assert
  base::MockCallback<BuildUserDataCallback> callback;
  EXPECT_CALL(callback, Run).WillOnce([](base::Value::Dict user_data) {
    std::string json;
    ASSERT_TRUE(base::JSONWriter::Write(user_data, &json));
    const std::string pattern =
        R"({"conversion":\[{"action":"click"},{"envelope":{"alg":"crypto_box_curve25519xsalsa20poly1305","ciphertext":".{64}","epk":".{44}","nonce":".{32}"}}]})";
    EXPECT_TRUE(RE2::FullMatch(json, pattern));
  });

  // Act
  BuildConversionUserData(kCreativeInstanceId, callback.Get());
}

TEST_F(HnsAdsConversionUserDataBuilderTest,
       DoNotBuildConversionUserDataForMissingCreativeInstanceId) {
  // Arrange
  BuildAndSaveConversionQueueItemsForTesting(
      AdType::kNotificationAd, ConfirmationType::kViewed,
      /*is_verifiable*/ false, /*should_use_random_uuids*/ false, /*count*/ 1);

  // Assert
  base::MockCallback<BuildUserDataCallback> callback;
  EXPECT_CALL(callback, Run(::testing::IsEmpty()));

  // Act
  BuildConversionUserData(kMissingCreativeInstanceId, callback.Get());
}

TEST_F(HnsAdsConversionUserDataBuilderTest,
       DoNotBuildVerifiableConversionUserDataForMissingCreativeInstanceId) {
  // Arrange
  BuildAndSaveConversionQueueItemsForTesting(
      AdType::kNotificationAd, ConfirmationType::kClicked,
      /*is_verifiable*/ true, /*should_use_random_uuids*/ false, /*count*/ 1);

  // Assert
  base::MockCallback<BuildUserDataCallback> callback;
  EXPECT_CALL(callback, Run(::testing::IsEmpty()));

  // Act
  BuildConversionUserData(kMissingCreativeInstanceId, callback.Get());
}

TEST_F(HnsAdsConversionUserDataBuilderTest,
       DoNotBuildConversionUserDataIfQueueIsEmpty) {
  // Arrange

  // Assert
  base::MockCallback<BuildUserDataCallback> callback;
  EXPECT_CALL(callback, Run(::testing::IsEmpty()));

  // Act
  BuildConversionUserData(kCreativeInstanceId, callback.Get());
}

}  // namespace hns_ads
