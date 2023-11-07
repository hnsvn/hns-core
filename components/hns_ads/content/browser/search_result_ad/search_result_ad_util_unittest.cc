/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/content/browser/search_result_ad/search_result_ad_util.h"

#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

namespace hns_ads {

TEST(SearchResultAdUtilTest, CheckGetPlacementIdFromSearchResultAdClickedUrl) {
  GURL url(
      "https://search.hns.com/a/"
      "redirect?click_url=https:%3A%2F%2Fexample.com&placement_id=placement-id&"
      "creative_instance_id=creative-id");
  EXPECT_EQ("placement-id", GetPlacementIdFromSearchResultAdClickedUrl(url));

  url = GURL(
      "https://safesearch.hns.com/a/"
      "redirect?click_url=https:%3A%2F%2Fexample.com&placement_id=placement-id&"
      "creative_instance_id=creative");
  EXPECT_EQ("placement-id", GetPlacementIdFromSearchResultAdClickedUrl(url));

  url = GURL(
      "https://search.hns.software/a/"
      "redirect?click_url=https:%3A%2F%2Fexample.com&placement_id=placement-id&"
      "creative_instance_id=creative");
  EXPECT_EQ("placement-id", GetPlacementIdFromSearchResultAdClickedUrl(url));

  url = GURL();
  EXPECT_FALSE(GetPlacementIdFromSearchResultAdClickedUrl(url).has_value());

  url = GURL(
      "http://search.hns.software/a/"
      "redirect?click_url=https:%3A%2F%2Fexample.com&placement_id=placement-id&"
      "creative_instance_id=creative");
  EXPECT_FALSE(GetPlacementIdFromSearchResultAdClickedUrl(url).has_value());

  url = GURL(
      "https://search.hns.software/b/"
      "redirect?click_url=https:%3A%2F%2Fexample.com&placement_id=placement-id&"
      "creative_instance_id=creative");
  EXPECT_FALSE(GetPlacementIdFromSearchResultAdClickedUrl(url).has_value());

  url = GURL(
      "https://search.hns.software/a/"
      "redirected?click_url=https:%3A%2F%2Fexample.com&placement_id=placement-"
      "id&creative_instance_id=creative");
  EXPECT_FALSE(GetPlacementIdFromSearchResultAdClickedUrl(url).has_value());

  url = GURL("https://search.hns.software/a/redirect?");
  EXPECT_FALSE(GetPlacementIdFromSearchResultAdClickedUrl(url).has_value());

  url = GURL(
      "https://hns.software/a/"
      "redirect?click_url=https:%3A%2F%2Fexample.com&placement_id=placement-id&"
      "creative_instance_id=creative");
  EXPECT_FALSE(GetPlacementIdFromSearchResultAdClickedUrl(url).has_value());

  url = GURL(
      "https://search.hns.software/a/"
      "redirect?click_url=https:%3A%2F%2Fexample.com&placement=placement-id&"
      "creative_instance_id=creative");
  EXPECT_FALSE(GetPlacementIdFromSearchResultAdClickedUrl(url).has_value());

  url = GURL(
      "https://search.hns.software/a/"
      "redirect?click_url=https:%3A%2F%2Fexample.com&placement_id&"
      "creative_instance_id=creative");
  EXPECT_EQ("", GetPlacementIdFromSearchResultAdClickedUrl(url));
}

}  // namespace hns_ads
