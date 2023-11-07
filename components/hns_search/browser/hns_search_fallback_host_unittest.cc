/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_search/browser/hns_search_fallback_host.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

namespace hns_search {

TEST(HnsSearchFallbackHost, GetBackupResultURL) {
  GURL base_url("https://www.google.com/search/");
  ASSERT_EQ(HnsSearchFallbackHost::GetBackupResultURL(base_url, "test", "en",
                                                        "ca", "32,32", true, 0),
            GURL("https://www.google.com/search/"
                 "?q=test&start=0&hl=en&gl=ca&safe=active"));
}

TEST(HnsSearchFallbackHost, GetBackupResultURLNoLang) {
  GURL base_url("https://www.google.com/search/");
  ASSERT_EQ(
      HnsSearchFallbackHost::GetBackupResultURL(base_url, "test", "", "ca",
                                                  "32,32", true, 0),
      GURL("https://www.google.com/search/?q=test&start=0&gl=ca&safe=active"));
}

TEST(HnsSearchFallbackHost, GetBackupResultURLNoCountry) {
  GURL base_url("https://www.google.com/search/");
  ASSERT_EQ(
      HnsSearchFallbackHost::GetBackupResultURL(base_url, "test", "en", "",
                                                  "32,32", true, 0),
      GURL("https://www.google.com/search/?q=test&start=0&hl=en&safe=active"));
}

TEST(HnsSearchFallbackHost, GetBackupResultURLNoFilter) {
  GURL base_url("https://www.google.com/search/");
  ASSERT_EQ(HnsSearchFallbackHost::GetBackupResultURL(
                base_url, "test", "en", "ca", "32,32", false, 0),
            GURL("https://www.google.com/search/?q=test&start=0&hl=en&gl=ca"));
}

TEST(HnsSearchFallbackHost, GetBackupResultURLMinimal) {
  GURL base_url("https://www.google.com/search/");
  ASSERT_EQ(HnsSearchFallbackHost::GetBackupResultURL(base_url, "test", "",
                                                        "", "", false, 0),
            GURL("https://www.google.com/search/?q=test&start=0"));
}

TEST(HnsSearchFallbackHost, GetBackupResultURLPageIndex) {
  GURL base_url("https://www.google.com/search/");
  ASSERT_EQ(HnsSearchFallbackHost::GetBackupResultURL(
                base_url, "test", "en", "ca", "32,32", false, 30),
            GURL("https://www.google.com/search/?q=test&start=30&hl=en&gl=ca"));
}

}  // namespace hns_search
