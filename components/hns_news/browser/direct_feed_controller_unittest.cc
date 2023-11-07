// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include <vector>

#include "hns/components/hns_news/browser/hns_news_controller.h"
#include "hns/components/hns_news/browser/direct_feed_controller.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "components/prefs/testing_pref_service.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace hns_news {

TEST(HnsNewsDirectFeedController, CanAddDirectFeed) {
  TestingPrefServiceSimple prefs;
  HnsNewsController::RegisterProfilePrefs(prefs.registry());

  DirectFeedController controller(&prefs, nullptr);

  EXPECT_TRUE(
      controller.AddDirectFeedPref(GURL("https://example.com"), "Example"));
}

TEST(HnsNewsDirectFeedController, CantAddDuplicateFeed) {
  TestingPrefServiceSimple prefs;
  HnsNewsController::RegisterProfilePrefs(prefs.registry());

  DirectFeedController controller(&prefs, nullptr);

  EXPECT_TRUE(
      controller.AddDirectFeedPref(GURL("https://example.com"), "Example 1"));
  EXPECT_FALSE(
      controller.AddDirectFeedPref(GURL("https://example.com"), "Example 2"));
}

TEST(HnsNewsDirectFeedController, CanExplicitlySetId) {
  TestingPrefServiceSimple prefs;
  HnsNewsController::RegisterProfilePrefs(prefs.registry());

  DirectFeedController controller(&prefs, nullptr);

  constexpr char kDirectFeedId[] = "1234";
  EXPECT_TRUE(controller.AddDirectFeedPref(GURL("https://example.com"),
                                           "Example", kDirectFeedId));

  const auto& dict = prefs.GetDict(prefs::kHnsNewsDirectFeeds);
  ASSERT_EQ(1u, dict.size());
  for (const auto&& [key, value] : dict) {
    EXPECT_EQ(key, kDirectFeedId);
  }
}

TEST(HnsNewsDirectFeedController, EmptyTitleFallsBackToFeedSource) {
  TestingPrefServiceSimple prefs;
  HnsNewsController::RegisterProfilePrefs(prefs.registry());

  DirectFeedController controller(&prefs, nullptr);

  constexpr char kFeedSource[] = "https://example.com/";
  EXPECT_TRUE(controller.AddDirectFeedPref(GURL(kFeedSource), ""));

  const auto& dict = prefs.GetDict(prefs::kHnsNewsDirectFeeds);
  ASSERT_EQ(1u, dict.size());
  for (const auto&& [key, value] : dict) {
    auto* title =
        value.GetDict().FindString(prefs::kHnsNewsDirectFeedsKeyTitle);
    EXPECT_EQ(kFeedSource, *title);
  }
}

TEST(HnsNewsDirectFeedController, DirectFeedCanBeInspectedAndRemoved) {
  TestingPrefServiceSimple prefs;
  HnsNewsController::RegisterProfilePrefs(prefs.registry());

  DirectFeedController controller(&prefs, nullptr);

  EXPECT_TRUE(
      controller.AddDirectFeedPref(GURL("https://example.com"), "Example"));

  auto parsed = controller.ParseDirectFeedsPref();
  EXPECT_EQ(parsed.size(), 1u);
  EXPECT_NE("", parsed[0]->publisher_id);
  EXPECT_EQ("Example", parsed[0]->publisher_name);
  EXPECT_EQ(GURL("https://example.com"), parsed[0]->feed_source);

  controller.RemoveDirectFeedPref(parsed[0]->publisher_id);
  parsed = controller.ParseDirectFeedsPref();
  EXPECT_EQ(0u, parsed.size());
}

}  // namespace hns_news
