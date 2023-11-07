/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/browser/publisher_utils.h"

#include <string>

#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

namespace hns_rewards {

class RewardsPublisherUtilsTest : public testing::Test {
 protected:
  absl::optional<std::string> GetPublisherId(const std::string& url) {
    return GetPublisherIdFromURL(GURL(url));
  }
};

TEST_F(RewardsPublisherUtilsTest, GetPublisherIdFromURL) {
  EXPECT_EQ(GetPublisherId("https://hns.com"), "hns.com");
  EXPECT_EQ(GetPublisherId("http://hns.com"), "hns.com");
  EXPECT_EQ(GetPublisherId("https://search.hns.com"), "hns.com");
  EXPECT_EQ(GetPublisherId("http://search.hns.com"), "hns.com");

  EXPECT_EQ(GetPublisherId("https://hns.co.uk"), "hns.co.uk");
  EXPECT_EQ(GetPublisherId("https://www.hns.co.uk"), "hns.co.uk");

  EXPECT_EQ(GetPublisherId("file:///a/b/c/"), absl::nullopt);
  EXPECT_EQ(GetPublisherId("invalid-url"), absl::nullopt);

  EXPECT_EQ(GetPublisherId("https://twitter.com/foo"), absl::nullopt);
  EXPECT_EQ(GetPublisherId("https://github.com/foo"), absl::nullopt);
  EXPECT_EQ(GetPublisherId("https://reddit.com/foo"), absl::nullopt);
  EXPECT_EQ(GetPublisherId("https://youtube.com/foo"), absl::nullopt);
  EXPECT_EQ(GetPublisherId("https://vimeo.com/foo"), absl::nullopt);
  EXPECT_EQ(GetPublisherId("https://twitch.tv/foo"), absl::nullopt);
}

}  // namespace hns_rewards
