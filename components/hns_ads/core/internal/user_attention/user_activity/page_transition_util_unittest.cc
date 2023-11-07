/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_attention/user_activity/page_transition_util.h"

#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsPageTransitionUtilTest, IsNewNavigation) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(IsNewNavigation(kPageTransitionTyped));
}

TEST(HnsAdsPageTransitionUtilTest,
     DidUseBackOrFowardButtonToTriggerNavigation) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(
      DidUseBackOrFowardButtonToTriggerNavigation(kPageTransitionForwardBack));
}

TEST(HnsAdsPageTransitionUtilTest, DidUseAddressBarToTriggerNavigation) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(
      DidUseAddressBarToTriggerNavigation(kPageTransitionFromAddressBar));
}

TEST(HnsAdsPageTransitionUtilTest, DidNavigateToHomePage) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(DidNavigateToHomePage(kPageTransitionHomePage));
}

TEST(HnsAdsPageTransitionUtilTest, DidTransitionFromExternalApplication) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(DidTransitionFromExternalApplication(kPageTransitionFromAPI));
}

TEST(HnsAdsPageTransitionUtilTest, ToUserActivityClickedLinkEventType) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(UserActivityEventType::kClickedLink,
            ToUserActivityEventType(kPageTransitionLink));
}

TEST(HnsAdsPageTransitionUtilTest, ToUserActivityTypedUrlEventType) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(UserActivityEventType::kTypedUrl,
            ToUserActivityEventType(kPageTransitionTyped));
}

TEST(HnsAdsPageTransitionUtilTest, ToUserActivityClickedBookmarkEventType) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(UserActivityEventType::kClickedBookmark,
            ToUserActivityEventType(kPageTransitionAutoBookmark));
}

TEST(HnsAdsPageTransitionUtilTest,
     ToUserActivityTypedAndSelectedNonUrlEventType) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(UserActivityEventType::kTypedAndSelectedNonUrl,
            ToUserActivityEventType(kPageTransitionGenerated));
}

TEST(HnsAdsPageTransitionUtilTest, ToUserActivitySubmittedFormEventType) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(UserActivityEventType::kSubmittedForm,
            ToUserActivityEventType(kPageTransitionFormSubmit));
}

TEST(HnsAdsPageTransitionUtilTest,
     ToUserActivityClickedReloadButtonEventType) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(UserActivityEventType::kClickedReloadButton,
            ToUserActivityEventType(kPageTransitionReload));
}

TEST(HnsAdsPageTransitionUtilTest,
     ToUserActivityTypedKeywordOtherThanDefaultSearchProviderEventType) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(UserActivityEventType::kTypedKeywordOtherThanDefaultSearchProvider,
            ToUserActivityEventType(kPageTransitionKeyword));
}

TEST(HnsAdsPageTransitionUtilTest, ToUserActivityGeneratedKeywordEventType) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(UserActivityEventType::kGeneratedKeyword,
            ToUserActivityEventType(kPageTransitionKeywordGenerated));
}

}  // namespace hns_ads
