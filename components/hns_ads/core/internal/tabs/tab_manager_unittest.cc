/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/tabs/tab_manager.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsTabManagerTest : public TabManagerObserver, public UnitTestBase {
 protected:
  void SetUp() override {
    UnitTestBase::SetUp();

    TabManager::GetInstance().AddObserver(this);
  }

  void TearDown() override {
    TabManager::GetInstance().RemoveObserver(this);

    UnitTestBase::TearDown();
  }

  void OnTabDidChangeFocus(const int32_t /*tab_id*/) override {
    tab_did_change_focus_ = true;
  }

  void OnTabDidChange(const TabInfo& /*tab*/) override {
    tab_did_change_ = true;
  }

  void OnDidOpenNewTab(const TabInfo& /*tab*/) override {
    did_open_new_tab_ = true;
  }

  void OnDidCloseTab(const int32_t /*tab_id*/) override {
    did_close_tab_ = true;
  }

  void OnTabDidStartPlayingMedia(const int32_t /*tab_id*/) override {
    tab_did_start_playing_media_ = true;
  }

  void OnTabDidStopPlayingMedia(const int32_t /*tab_id*/) override {
    tab_did_stop_playing_media_ = true;
  }

  void ResetObserver() {
    tab_did_change_focus_ = false;
    tab_did_change_ = false;
    did_open_new_tab_ = false;
    did_close_tab_ = false;
    tab_did_start_playing_media_ = false;
    tab_did_stop_playing_media_ = false;
  }

  bool tab_did_change_focus_ = false;
  bool tab_did_change_ = false;
  bool did_open_new_tab_ = false;
  bool did_close_tab_ = false;
  bool tab_did_start_playing_media_ = false;
  bool tab_did_stop_playing_media_ = false;
};

TEST_F(HnsAdsTabManagerTest, IsVisible) {
  // Arrange

  // Act
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_active*/ true);

  // Assert
  EXPECT_TRUE(TabManager::GetInstance().IsVisible(/*id*/ 1));
}

TEST_F(HnsAdsTabManagerTest, IsTabOccluded) {
  // Arrange

  // Act
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_visible*/ false);

  // Assert
  EXPECT_FALSE(TabManager::GetInstance().IsVisible(/*id*/ 1));
}

TEST_F(HnsAdsTabManagerTest, OpenNewTab) {
  // Arrange

  // Act
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_active*/ true);

  // Assert
  const absl::optional<TabInfo> tab =
      TabManager::GetInstance().MaybeGetForId(1);

  TabInfo expected_tab;
  expected_tab.id = 1;
  expected_tab.redirect_chain = {GURL("https://hns.com")};
  expected_tab.is_playing_media = false;
  EXPECT_EQ(expected_tab, tab);

  EXPECT_FALSE(tab_did_change_focus_);
  EXPECT_FALSE(tab_did_change_);
  EXPECT_TRUE(did_open_new_tab_);
  EXPECT_FALSE(did_close_tab_);
  EXPECT_FALSE(tab_did_start_playing_media_);
  EXPECT_FALSE(tab_did_stop_playing_media_);
}

TEST_F(HnsAdsTabManagerTest, ChangeTabFocus) {
  // Arrange
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_visible*/ false);
  ResetObserver();

  // Act
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_active*/ true);

  // Assert
  EXPECT_TRUE(tab_did_change_focus_);
  EXPECT_FALSE(tab_did_change_);
  EXPECT_FALSE(did_open_new_tab_);
  EXPECT_FALSE(did_close_tab_);
  EXPECT_FALSE(tab_did_start_playing_media_);
  EXPECT_FALSE(tab_did_stop_playing_media_);
}

TEST_F(HnsAdsTabManagerTest, DoNotUpdateExistingOccludedTabWithSameUrl) {
  // Arrange
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_visible*/ false);
  ResetObserver();

  // Act
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_visible*/ false);

  // Assert
  const absl::optional<TabInfo> tab =
      TabManager::GetInstance().MaybeGetForId(1);
  ASSERT_TRUE(tab);

  TabInfo expected_tab;
  expected_tab.id = 1;
  expected_tab.redirect_chain = {GURL("https://hns.com")};
  expected_tab.is_playing_media = false;
  EXPECT_EQ(expected_tab, tab);

  EXPECT_FALSE(tab_did_change_focus_);
  EXPECT_FALSE(tab_did_change_);
  EXPECT_FALSE(did_open_new_tab_);
  EXPECT_FALSE(did_close_tab_);
  EXPECT_FALSE(tab_did_start_playing_media_);
  EXPECT_FALSE(tab_did_stop_playing_media_);
}

TEST_F(HnsAdsTabManagerTest, UpdateExistingOccludedTabWithDifferentUrl) {
  // Arrange
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_visible*/ false);
  ResetObserver();

  // Act
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com/about")},
      /*is_visible*/ false);

  // Assert
  const absl::optional<TabInfo> tab =
      TabManager::GetInstance().MaybeGetForId(1);
  ASSERT_TRUE(tab);

  TabInfo expected_tab;
  expected_tab.id = 1;
  expected_tab.redirect_chain = {GURL("https://hns.com/about")};
  expected_tab.is_playing_media = false;
  EXPECT_EQ(expected_tab, tab);

  EXPECT_FALSE(tab_did_change_focus_);
  EXPECT_TRUE(tab_did_change_);
  EXPECT_FALSE(did_open_new_tab_);
  EXPECT_FALSE(did_close_tab_);
  EXPECT_FALSE(tab_did_start_playing_media_);
  EXPECT_FALSE(tab_did_stop_playing_media_);
}

TEST_F(HnsAdsTabManagerTest, DoNotUpdateExistingTabWithSameUrl) {
  // Arrange
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_active*/ true);
  ResetObserver();

  // Act
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_active*/ true);

  // Assert
  const absl::optional<TabInfo> tab =
      TabManager::GetInstance().MaybeGetForId(1);
  ASSERT_TRUE(tab);

  TabInfo expected_tab;
  expected_tab.id = 1;
  expected_tab.redirect_chain = {GURL("https://hns.com")};
  expected_tab.is_playing_media = false;
  EXPECT_EQ(expected_tab, tab);

  EXPECT_FALSE(tab_did_change_focus_);
  EXPECT_FALSE(tab_did_change_);
  EXPECT_FALSE(did_open_new_tab_);
  EXPECT_FALSE(did_close_tab_);
  EXPECT_FALSE(tab_did_start_playing_media_);
  EXPECT_FALSE(tab_did_stop_playing_media_);
}

TEST_F(HnsAdsTabManagerTest, UpdatedExistingTabWithDifferentUrl) {
  // Arrange
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_active*/ true);
  ResetObserver();

  // Act
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com/about")},
      /*is_visible*/ true);

  // Assert
  const absl::optional<TabInfo> tab =
      TabManager::GetInstance().MaybeGetForId(1);
  ASSERT_TRUE(tab);

  TabInfo expected_tab;
  expected_tab.id = 1;
  expected_tab.redirect_chain = {GURL("https://hns.com/about")};
  expected_tab.is_playing_media = false;
  EXPECT_EQ(expected_tab, tab);

  EXPECT_FALSE(tab_did_change_focus_);
  EXPECT_TRUE(tab_did_change_);
  EXPECT_FALSE(did_open_new_tab_);
  EXPECT_FALSE(did_close_tab_);
  EXPECT_FALSE(tab_did_start_playing_media_);
  EXPECT_FALSE(tab_did_stop_playing_media_);
}

TEST_F(HnsAdsTabManagerTest, CloseTab) {
  // Arrange
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_active*/ true);
  ResetObserver();

  // Act
  NotifyDidCloseTab(/*id*/ 1);

  // Assert
  EXPECT_FALSE(TabManager::GetInstance().MaybeGetForId(1));

  EXPECT_FALSE(tab_did_change_focus_);
  EXPECT_FALSE(tab_did_change_);
  EXPECT_FALSE(did_open_new_tab_);
  EXPECT_TRUE(did_close_tab_);
  EXPECT_FALSE(tab_did_start_playing_media_);
  EXPECT_FALSE(tab_did_stop_playing_media_);
}

TEST_F(HnsAdsTabManagerTest, PlayMedia) {
  // Arrange
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://foobar.com")},
      /*is_visible*/ true);
  ResetObserver();

  // Act
  NotifyTabDidStartPlayingMedia(/*tab_id*/ 1);

  // Assert
  EXPECT_TRUE(TabManager::GetInstance().IsPlayingMedia(1));

  EXPECT_FALSE(tab_did_change_focus_);
  EXPECT_FALSE(tab_did_change_);
  EXPECT_FALSE(did_open_new_tab_);
  EXPECT_FALSE(did_close_tab_);
  EXPECT_TRUE(tab_did_start_playing_media_);
  EXPECT_FALSE(tab_did_stop_playing_media_);
}

TEST_F(HnsAdsTabManagerTest, AlreadyPlayingMedia) {
  // Arrange
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://foobar.com")},
      /*is_visible*/ true);
  NotifyTabDidStartPlayingMedia(/*tab_id*/ 1);
  ResetObserver();

  // Act
  NotifyTabDidStartPlayingMedia(/*tab_id*/ 1);

  // Assert
  EXPECT_TRUE(TabManager::GetInstance().IsPlayingMedia(1));

  EXPECT_FALSE(tab_did_change_focus_);
  EXPECT_FALSE(tab_did_change_);
  EXPECT_FALSE(did_open_new_tab_);
  EXPECT_FALSE(did_close_tab_);
  EXPECT_FALSE(tab_did_start_playing_media_);
  EXPECT_FALSE(tab_did_stop_playing_media_);
}

TEST_F(HnsAdsTabManagerTest, StopPlayingMedia) {
  // Arrange
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_active*/ true);
  NotifyTabDidStartPlayingMedia(/*tab_id*/ 1);
  ResetObserver();

  // Act
  NotifyTabDidStopPlayingMedia(/*id*/ 1);

  // Assert
  EXPECT_FALSE(TabManager::GetInstance().IsPlayingMedia(1));

  EXPECT_FALSE(tab_did_change_focus_);
  EXPECT_FALSE(tab_did_change_);
  EXPECT_FALSE(did_open_new_tab_);
  EXPECT_FALSE(did_close_tab_);
  EXPECT_FALSE(tab_did_start_playing_media_);
  EXPECT_TRUE(tab_did_stop_playing_media_);
}

TEST_F(HnsAdsTabManagerTest, GetVisible) {
  // Arrange
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://foobar.com")},
      /*is_visible*/ true);
  NotifyTabDidChange(
      /*id*/ 2, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_active*/ true);

  // Act
  const absl::optional<TabInfo> tab = TabManager::GetInstance().GetVisible();
  ASSERT_TRUE(tab);

  // Assert
  TabInfo expected_tab;
  expected_tab.id = 2;
  expected_tab.redirect_chain = {GURL("https://hns.com")};
  expected_tab.is_playing_media = false;

  EXPECT_EQ(expected_tab, tab);
}

TEST_F(HnsAdsTabManagerTest, GetLastVisible) {
  // Arrange
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://foobar.com")},
      /*is_visible*/ true);
  NotifyTabDidChange(
      /*id*/ 2, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_active*/ true);

  // Act
  const absl::optional<TabInfo> tab =
      TabManager::GetInstance().GetLastVisible();
  ASSERT_TRUE(tab);

  // Assert
  TabInfo expected_tab;
  expected_tab.id = 1;
  expected_tab.redirect_chain = {GURL("https://foobar.com")};
  expected_tab.is_playing_media = false;

  EXPECT_EQ(expected_tab, tab);
}

TEST_F(HnsAdsTabManagerTest, GetForId) {
  // Arrange
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_active*/ true);

  // Act
  const absl::optional<TabInfo> tab =
      TabManager::GetInstance().MaybeGetForId(1);
  ASSERT_TRUE(tab);

  // Assert
  TabInfo expected_tab;
  expected_tab.id = 1;
  expected_tab.redirect_chain = {GURL("https://hns.com")};
  expected_tab.is_playing_media = false;

  EXPECT_EQ(expected_tab, tab);
}

TEST_F(HnsAdsTabManagerTest, DoNotGetTabForMissingId) {
  // Arrange
  NotifyTabDidChange(
      /*id*/ 1, /*redirect_chain*/ {GURL("https://hns.com")},
      /*is_active*/ true);

  // Act

  // Assert
  EXPECT_FALSE(TabManager::GetInstance().MaybeGetForId(2));
}

}  // namespace hns_ads
