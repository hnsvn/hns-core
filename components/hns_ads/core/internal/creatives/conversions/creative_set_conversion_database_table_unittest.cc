/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/creatives/conversions/creative_set_conversion_database_table.h"

#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_container_util.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/creatives/conversions/creative_set_conversion_database_table_util.h"
#include "hns/components/hns_ads/core/internal/creatives/conversions/creative_set_conversion_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCreativeSetConversionDatabaseTableTest : public UnitTestBase {
 protected:
  database::table::CreativeSetConversions database_table_;
};

TEST_F(HnsAdsCreativeSetConversionDatabaseTableTest, EmptySave) {
  // Arrange

  // Act
  database::SaveCreativeSetConversions({});

  // Assert
  database_table_.GetAll(base::BindOnce(
      [](const bool success,
         const CreativeSetConversionList& creative_set_conversions) {
        ASSERT_TRUE(success);
        EXPECT_TRUE(creative_set_conversions.empty());
      }));
}

TEST_F(HnsAdsCreativeSetConversionDatabaseTableTest,
       SaveCreativeSetConversions) {
  // Arrange
  CreativeSetConversionList creative_set_conversions;

  const CreativeSetConversionInfo creative_set_conversion_1 =
      BuildVerifiableCreativeSetConversionForTesting(
          kCreativeSetId, /*url_pattern*/ "https://www.hns.com/*",
          /*observation_window*/ base::Days(3),
          kVerifiableConversionAdvertiserPublicKey);
  creative_set_conversions.push_back(creative_set_conversion_1);

  const CreativeSetConversionInfo creative_set_conversion_2 =
      BuildCreativeSetConversionForTesting(
          /*creative_set_id*/ "4e83a23c-1194-40f8-8fdc-2f38d7ed75c8",
          /*url_pattern*/ "https://www.hns.com/signup/*",
          /*observation_window*/ base::Days(30));
  creative_set_conversions.push_back(creative_set_conversion_2);

  // Act
  database::SaveCreativeSetConversions(creative_set_conversions);

  // Assert
  database_table_.GetAll(base::BindOnce(
      [](const CreativeSetConversionList& expected_creative_set_conversions,
         const bool success,
         const CreativeSetConversionList& creative_set_conversions) {
        ASSERT_TRUE(success);
        EXPECT_TRUE(ContainersEq(expected_creative_set_conversions,
                                 creative_set_conversions));
      },
      creative_set_conversions));
}

TEST_F(HnsAdsCreativeSetConversionDatabaseTableTest,
       DoNotSaveDuplicateConversion) {
  // Arrange
  CreativeSetConversionList creative_set_conversions;

  const CreativeSetConversionInfo creative_set_conversion =
      BuildVerifiableCreativeSetConversionForTesting(
          kCreativeSetId, /*url_pattern*/ "https://www.hns.com/*",
          /*observation_window*/ base::Days(3),
          kVerifiableConversionAdvertiserPublicKey);
  creative_set_conversions.push_back(creative_set_conversion);

  database::SaveCreativeSetConversions(creative_set_conversions);

  // Act
  database::SaveCreativeSetConversions(creative_set_conversions);

  // Assert
  database_table_.GetAll(base::BindOnce(
      [](const CreativeSetConversionList& expected_creative_set_conversions,
         const bool success,
         const CreativeSetConversionList& creative_set_conversions) {
        ASSERT_TRUE(success);
        EXPECT_EQ(expected_creative_set_conversions, creative_set_conversions);
      },
      creative_set_conversions));
}

TEST_F(HnsAdsCreativeSetConversionDatabaseTableTest,
       PurgeExpiredConversions) {
  // Arrange
  CreativeSetConversionList creative_set_conversions;

  const CreativeSetConversionInfo creative_set_conversion_1 =
      BuildCreativeSetConversionForTesting(
          kCreativeSetId,
          /*url_pattern*/ "https://www.hns.com/*",
          /*observation_window*/ base::Days(7));
  creative_set_conversions.push_back(creative_set_conversion_1);

  const CreativeSetConversionInfo creative_set_conversion_2 =
      BuildCreativeSetConversionForTesting(
          /*creative_set_id*/ "4e83a23c-1194-40f8-8fdc-2f38d7ed75c8",
          /*url_pattern*/ "https://www.hns.com/signup/*",
          /*observation_window*/ base::Days(3));  // Should be purged
  creative_set_conversions.push_back(creative_set_conversion_2);

  database::SaveCreativeSetConversions(creative_set_conversions);

  // Act
  AdvanceClockBy(base::Days(4));

  database::PurgeExpiredCreativeSetConversions();

  // Assert
  const CreativeSetConversionList expected_creative_set_conversions = {
      creative_set_conversion_1};

  database_table_.GetAll(base::BindOnce(
      [](const CreativeSetConversionList& expected_creative_set_conversions,
         const bool success,
         const CreativeSetConversionList& creative_set_conversions) {
        ASSERT_TRUE(success);
        EXPECT_TRUE(ContainersEq(expected_creative_set_conversions,
                                 creative_set_conversions));
      },
      expected_creative_set_conversions));
}

TEST_F(HnsAdsCreativeSetConversionDatabaseTableTest,
       SaveConversionWithMatchingCreativeSetIdAndType) {
  // Arrange
  CreativeSetConversionList creative_set_conversions_1;

  const CreativeSetConversionInfo creative_set_conversion_1 =
      BuildVerifiableCreativeSetConversionForTesting(
          kCreativeSetId,
          /*url_pattern*/ "https://www.hns.com/1",
          /*observation_window*/ base::Days(3),
          kVerifiableConversionAdvertiserPublicKey);
  creative_set_conversions_1.push_back(creative_set_conversion_1);

  database::SaveCreativeSetConversions(creative_set_conversions_1);

  // Act
  CreativeSetConversionList creative_set_conversions_2;

  const CreativeSetConversionInfo creative_set_conversion_2 =
      BuildVerifiableCreativeSetConversionForTesting(
          kCreativeSetId,
          /*url_pattern*/ "https://www.hns.com/2",
          /*observation_window*/ base::Days(30),
          kVerifiableConversionAdvertiserPublicKey);
  creative_set_conversions_2.push_back(creative_set_conversion_2);

  database::SaveCreativeSetConversions(creative_set_conversions_2);

  // Assert
  const CreativeSetConversionList expected_creative_set_conversions = {
      creative_set_conversion_2};

  database_table_.GetAll(base::BindOnce(
      [](const CreativeSetConversionList& expected_creative_set_conversions,
         const bool success,
         const CreativeSetConversionList& creative_set_conversions) {
        ASSERT_TRUE(success);
        EXPECT_EQ(expected_creative_set_conversions, creative_set_conversions);
      },
      expected_creative_set_conversions));
}

TEST_F(HnsAdsCreativeSetConversionDatabaseTableTest, TableName) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ("creative_set_conversions", database_table_.GetTableName());
}

}  // namespace hns_ads
