/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/segments/segment_util.h"

#include "hns/components/hns_ads/core/internal/catalog/catalog_info.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_json_reader.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_file_util.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/client_state_manager.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"
#include "hns/components/hns_ads/core/public/history/category_content_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {
constexpr char kCatalog[] = "catalog_with_multiple_campaigns.json";
}  // namespace

class HnsAdsSegmentUtilTest : public UnitTestBase {};

TEST_F(HnsAdsSegmentUtilTest, GetSegmentsFromCatalog) {
  // Arrange
  const absl::optional<std::string> json =
      ReadFileFromTestPathToString(kCatalog);
  ASSERT_TRUE(json);

  const absl::optional<CatalogInfo> catalog = json::reader::ReadCatalog(*json);
  ASSERT_TRUE(catalog);

  // Act
  const SegmentList segments = GetSegments(*catalog);

  // Assert
  const SegmentList expected_segments = {"technology & computing",
                                         "untargeted"};
  EXPECT_EQ(expected_segments, segments);
}

TEST_F(HnsAdsSegmentUtilTest, GetSegmentsFromEmptyCatalog) {
  // Arrange

  // Act
  const SegmentList segments = GetSegments(/*catalog*/ {});

  // Assert
  EXPECT_TRUE(segments.empty());
}

TEST_F(HnsAdsSegmentUtilTest, GetParentSegmentFromParentChildSegment) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ("technology & computing",
            GetParentSegment("technology & computing-software"));
}

TEST_F(HnsAdsSegmentUtilTest, GetParentSegmentFromParentSegment) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ("technology & computing",
            GetParentSegment("technology & computing"));
}

TEST_F(HnsAdsSegmentUtilTest, GetParentSegments) {
  // Arrange
  const SegmentList segments = {"technology & computing-software",
                                "personal finance-personal finance",
                                "automobiles"};

  // Act
  const SegmentList parent_segments = GetParentSegments(segments);

  // Assert
  const SegmentList expected_parent_segments = {
      "technology & computing", "personal finance", "automobiles"};
  EXPECT_EQ(expected_parent_segments, parent_segments);
}

TEST_F(HnsAdsSegmentUtilTest, GetParentSegmentsForEmptyList) {
  // Arrange

  // Act
  const SegmentList parent_segments = GetParentSegments(/*segments*/ {});

  // Assert
  EXPECT_TRUE(parent_segments.empty());
}

TEST_F(HnsAdsSegmentUtilTest, ShouldFilterMatchingParentChildSegment) {
  // Arrange
  CategoryContentInfo category_content;
  category_content.category = "parent-child";
  category_content.user_reaction_type = mojom::UserReactionType::kNeutral;

  // Act
  ClientStateManager::GetInstance().ToggleDislikeCategory(category_content);

  // Assert
  EXPECT_TRUE(ShouldFilterSegment("parent-child"));
}

TEST_F(HnsAdsSegmentUtilTest, ShouldNotFilterNonMatchingParentChildSegment) {
  // Arrange
  CategoryContentInfo category_content;
  category_content.category = "parent-child";
  category_content.user_reaction_type = mojom::UserReactionType::kNeutral;

  // Act
  ClientStateManager::GetInstance().ToggleDislikeCategory(category_content);

  // Assert
  EXPECT_FALSE(ShouldFilterSegment("foo-bar"));
}

TEST_F(HnsAdsSegmentUtilTest, ShouldFilterMatchingParentSegment) {
  // Arrange
  CategoryContentInfo category_content;
  category_content.category = "parent";
  category_content.user_reaction_type = mojom::UserReactionType::kNeutral;

  // Act
  ClientStateManager::GetInstance().ToggleDislikeCategory(category_content);

  // Assert
  EXPECT_TRUE(ShouldFilterSegment("parent"));
}

TEST_F(HnsAdsSegmentUtilTest, ShouldNotFilterNonMatchingParentSegment) {
  // Arrange
  CategoryContentInfo category_content;
  category_content.category = "parent";
  category_content.user_reaction_type = mojom::UserReactionType::kNeutral;

  // Act
  ClientStateManager::GetInstance().ToggleDislikeCategory(category_content);

  // Assert
  EXPECT_FALSE(ShouldFilterSegment("foo"));
}

TEST_F(HnsAdsSegmentUtilTest,
       ShouldFilterAgainstParentForMatchingParentSegmentWithChild) {
  // Arrange
  CategoryContentInfo category_content;
  category_content.category = "parent";
  category_content.user_reaction_type = mojom::UserReactionType::kNeutral;

  // Act
  ClientStateManager::GetInstance().ToggleDislikeCategory(category_content);

  // Assert
  EXPECT_TRUE(ShouldFilterSegment("parent-child"));
}

TEST_F(HnsAdsSegmentUtilTest,
       ShouldNotFilterAgainstParentForNonMatchingParentSegmentWithChild) {
  // Arrange
  CategoryContentInfo category_content;
  category_content.category = "parent";
  category_content.user_reaction_type = mojom::UserReactionType::kNeutral;

  // Act
  ClientStateManager::GetInstance().ToggleDislikeCategory(category_content);

  // Assert
  EXPECT_FALSE(ShouldFilterSegment("foo-bar"));
}

TEST_F(HnsAdsSegmentUtilTest, HasChildSegment) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(HasChildSegment("technology & computing-windows"));
}

TEST_F(HnsAdsSegmentUtilTest, DoesNotHaveChildSegment) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(HasChildSegment("technology & computing"));
}

}  // namespace hns_ads
