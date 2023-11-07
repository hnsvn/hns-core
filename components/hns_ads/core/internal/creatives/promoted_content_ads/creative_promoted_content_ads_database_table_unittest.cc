/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/creatives/promoted_content_ads/creative_promoted_content_ads_database_table.h"

#include <utility>

#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_container_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/creatives/promoted_content_ads/creative_promoted_content_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/promoted_content_ads/creative_promoted_content_ads_database_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::database::table {

class HnsAdsCreativePromotedContentAdsDatabaseTableTest
    : public UnitTestBase {
 protected:
  CreativePromotedContentAds database_table_;
};

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableTest,
       SaveEmptyCreativePromotedContentAds) {
  // Arrange

  // Act
  database::SaveCreativePromotedContentAds({});

  // Assert
  database_table_.GetAll(
      base::BindOnce([](const bool success, const SegmentList& /*segments*/,
                        const CreativePromotedContentAdList& creative_ads) {
        ASSERT_TRUE(success);
        EXPECT_TRUE(creative_ads.empty());
      }));
}

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableTest,
       SaveCreativePromotedContentAds) {
  // Arrange
  const CreativePromotedContentAdList creative_ads =
      BuildCreativePromotedContentAdsForTesting(/*count*/ 2);

  // Act
  database::SaveCreativePromotedContentAds(creative_ads);

  // Assert
  database_table_.GetAll(base::BindOnce(
      [](const CreativePromotedContentAdList& expected_creative_ads,
         const bool success, const SegmentList& /*segments*/,
         const CreativePromotedContentAdList& creative_ads) {
        EXPECT_TRUE(success);
        EXPECT_TRUE(ContainersEq(expected_creative_ads, creative_ads));
      },
      creative_ads));
}

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableTest,
       SaveCreativePromotedContentAdsInBatches) {
  // Arrange
  database_table_.SetBatchSize(2);

  const CreativePromotedContentAdList creative_ads =
      BuildCreativePromotedContentAdsForTesting(/*count*/ 3);

  // Act
  database::SaveCreativePromotedContentAds(creative_ads);

  // Assert
  database_table_.GetAll(base::BindOnce(
      [](const CreativePromotedContentAdList& expected_creative_ads,
         const bool success, const SegmentList& /*segments*/,
         const CreativePromotedContentAdList& creative_ads) {
        EXPECT_TRUE(success);
        EXPECT_TRUE(ContainersEq(expected_creative_ads, creative_ads));
      },
      creative_ads));
}

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableTest,
       DoNotSaveDuplicateCreativePromotedContentAds) {
  // Arrange
  CreativePromotedContentAdList creative_ads;

  const CreativePromotedContentAdInfo creative_ad =
      BuildCreativePromotedContentAdForTesting(
          /*should_use_random_uuids*/ true);
  creative_ads.push_back(creative_ad);

  database::SaveCreativePromotedContentAds(creative_ads);

  // Act
  database::SaveCreativePromotedContentAds(creative_ads);

  // Assert
  database_table_.GetAll(base::BindOnce(
      [](const CreativePromotedContentAdList& expected_creative_ads,
         const bool success, const SegmentList& /*segments*/,
         const CreativePromotedContentAdList& creative_ads) {
        EXPECT_TRUE(success);
        EXPECT_EQ(expected_creative_ads, creative_ads);
      },
      creative_ads));
}

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableTest, GetForSegments) {
  // Arrange
  CreativePromotedContentAdList creative_ads;

  CreativePromotedContentAdInfo creative_ad_1 =
      BuildCreativePromotedContentAdForTesting(
          /*should_use_random_uuids*/ true);
  creative_ad_1.segment = "food & drink";
  creative_ads.push_back(creative_ad_1);

  CreativePromotedContentAdInfo creative_ad_2 =
      BuildCreativePromotedContentAdForTesting(
          /*should_use_random_uuids*/ true);
  creative_ad_2.segment = "technology & computing-software";
  creative_ads.push_back(creative_ad_2);

  CreativePromotedContentAdInfo creative_ad_3 =
      BuildCreativePromotedContentAdForTesting(
          /*should_use_random_uuids*/ true);
  creative_ad_3.segment = "food & drink";
  creative_ads.push_back(creative_ad_3);

  database::SaveCreativePromotedContentAds(creative_ads);

  // Act

  // Assert
  CreativePromotedContentAdList expected_creative_ads = {creative_ad_1,
                                                         creative_ad_3};

  database_table_.GetForSegments(
      /*segments*/ {"food & drink"},
      base::BindOnce(
          [](const CreativePromotedContentAdList& expected_creative_ads,
             const bool success, const SegmentList& /*segments*/,
             const CreativePromotedContentAdList& creative_ads) {
            EXPECT_TRUE(success);
            EXPECT_TRUE(ContainersEq(expected_creative_ads, creative_ads));
          },
          std::move(expected_creative_ads)));
}

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableTest,
       GetCreativePromotedContentAdsForCreativeInstanceId) {
  // Arrange
  CreativePromotedContentAdList creative_ads;

  const CreativePromotedContentAdInfo creative_ad_1 =
      BuildCreativePromotedContentAdForTesting(
          /*should_use_random_uuids*/ true);
  creative_ads.push_back(creative_ad_1);

  const CreativePromotedContentAdInfo creative_ad_2 =
      BuildCreativePromotedContentAdForTesting(
          /*should_use_random_uuids*/ true);
  creative_ads.push_back(creative_ad_2);

  database::SaveCreativePromotedContentAds(creative_ads);

  // Act

  // Assert
  database_table_.GetForCreativeInstanceId(
      creative_ad_1.creative_instance_id,
      base::BindOnce(
          [](const CreativePromotedContentAdInfo& expected_creative_ad,
             const bool success, const std::string& /*creative_instance_id*/,
             const CreativePromotedContentAdInfo& creative_ad) {
            ASSERT_TRUE(success);
            EXPECT_EQ(expected_creative_ad, creative_ad);
          },
          creative_ad_1));
}

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableTest,
       GetCreativePromotedContentAdsForNonExistentCreativeInstanceId) {
  // Arrange
  const CreativePromotedContentAdList creative_ads =
      BuildCreativePromotedContentAdsForTesting(/*count*/ 1);

  database::SaveCreativePromotedContentAds(creative_ads);

  // Act

  // Assert
  database_table_.GetForCreativeInstanceId(
      kMissingCreativeInstanceId,
      base::BindOnce([](const bool success,
                        const std::string& /*creative_instance_id*/,
                        const CreativePromotedContentAdInfo&
                        /*creative_ads*/) { EXPECT_FALSE(success); }));
}

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableTest,
       GetCreativePromotedContentAdsForEmptySegments) {
  // Arrange
  const CreativePromotedContentAdList creative_ads =
      BuildCreativePromotedContentAdsForTesting(/*count*/ 1);

  database::SaveCreativePromotedContentAds(creative_ads);

  // Act

  // Assert
  database_table_.GetForSegments(
      /*segments*/ {},
      base::BindOnce([](const bool success, const SegmentList& /*segments*/,
                        const CreativePromotedContentAdList& creative_ads) {
        ASSERT_TRUE(success);
        EXPECT_TRUE(creative_ads.empty());
      }));
}

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableTest,
       GetCreativePromotedContentAdsForNonExistentSegment) {
  // Arrange
  const CreativePromotedContentAdList creative_ads =
      BuildCreativePromotedContentAdsForTesting(/*count*/ 1);

  database::SaveCreativePromotedContentAds(creative_ads);

  // Act

  // Assert
  database_table_.GetForSegments(
      /*segments*/ {"FOOBAR"},
      base::BindOnce([](const bool success, const SegmentList& /*segments*/,
                        const CreativePromotedContentAdList& creative_ads) {
        ASSERT_TRUE(success);
        EXPECT_TRUE(creative_ads.empty());
      }));
}

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableTest,
       GetCreativePromotedContentAdsFromMultipleSegments) {
  // Arrange
  CreativePromotedContentAdList creative_ads;

  CreativePromotedContentAdInfo creative_ad_1 =
      BuildCreativePromotedContentAdForTesting(
          /*should_use_random_uuids*/ true);
  creative_ad_1.segment = "technology & computing-software";
  creative_ads.push_back(creative_ad_1);

  CreativePromotedContentAdInfo creative_ad_2 =
      BuildCreativePromotedContentAdForTesting(
          /*should_use_random_uuids*/ true);
  creative_ad_2.segment = "food & drink";
  creative_ads.push_back(creative_ad_2);

  CreativePromotedContentAdInfo creative_ad_3 =
      BuildCreativePromotedContentAdForTesting(
          /*should_use_random_uuids*/ true);
  creative_ad_3.segment = "automobiles";
  creative_ads.push_back(creative_ad_3);

  database::SaveCreativePromotedContentAds(creative_ads);

  // Act

  // Assert
  CreativePromotedContentAdList expected_creative_ads = {creative_ad_1,
                                                         creative_ad_2};

  database_table_.GetForSegments(
      /*segments*/ {creative_ad_1.segment, creative_ad_2.segment},
      base::BindOnce(
          [](const CreativePromotedContentAdList& expected_creative_ads,
             const bool success, const SegmentList& /*segments*/,
             const CreativePromotedContentAdList& creative_ads) {
            EXPECT_TRUE(success);
            EXPECT_TRUE(ContainersEq(expected_creative_ads, creative_ads));
          },
          std::move(expected_creative_ads)));
}

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableTest,
       GetNonExpiredCreativePromotedContentAds) {
  // Arrange
  CreativePromotedContentAdList creative_ads;

  CreativePromotedContentAdInfo creative_ad_1 =
      BuildCreativePromotedContentAdForTesting(
          /*should_use_random_uuids*/ true);
  creative_ad_1.start_at = DistantPast();
  creative_ad_1.end_at = Now();
  creative_ads.push_back(creative_ad_1);

  CreativePromotedContentAdInfo creative_ad_2 =
      BuildCreativePromotedContentAdForTesting(
          /*should_use_random_uuids*/ true);
  creative_ad_2.start_at = DistantPast();
  creative_ad_2.end_at = DistantFuture();
  creative_ads.push_back(creative_ad_2);

  database::SaveCreativePromotedContentAds(creative_ads);

  // Act
  AdvanceClockBy(base::Hours(1));

  // Assert
  CreativePromotedContentAdList expected_creative_ads = {creative_ad_2};

  database_table_.GetAll(base::BindOnce(
      [](const CreativePromotedContentAdList& expected_creative_ads,
         const bool success, const SegmentList& /*segments*/,
         const CreativePromotedContentAdList& creative_ads) {
        ASSERT_TRUE(success);
        EXPECT_EQ(expected_creative_ads, creative_ads);
      },
      std::move(expected_creative_ads)));
}

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableTest,
       GetCreativePromotedContentAdsMatchingCaseInsensitiveSegments) {
  // Arrange
  CreativePromotedContentAdList creative_ads;

  CreativePromotedContentAdInfo creative_ad_1 =
      BuildCreativePromotedContentAdForTesting(
          /*should_use_random_uuids*/ true);
  creative_ad_1.segment = "technology & computing-software";
  creative_ads.push_back(creative_ad_1);

  CreativePromotedContentAdInfo creative_ad_2 =
      BuildCreativePromotedContentAdForTesting(
          /*should_use_random_uuids*/ true);
  creative_ad_2.segment = "food & drink";
  creative_ads.push_back(creative_ad_2);

  database::SaveCreativePromotedContentAds(creative_ads);

  // Act

  // Assert
  CreativePromotedContentAdList expected_creative_ads = {creative_ad_2};

  database_table_.GetForSegments(
      /*segments*/ {"FoOd & DrInK"},
      base::BindOnce(
          [](const CreativePromotedContentAdList& expected_creative_ads,
             const bool success, const SegmentList& /*segments*/,
             const CreativePromotedContentAdList& creative_ads) {
            EXPECT_TRUE(success);
            EXPECT_EQ(expected_creative_ads, creative_ads);
          },
          std::move(expected_creative_ads)));
}

TEST_F(HnsAdsCreativePromotedContentAdsDatabaseTableTest, TableName) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ("creative_promoted_content_ads", database_table_.GetTableName());
}

}  // namespace hns_ads::database::table
