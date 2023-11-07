/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_shields/browser/filter_list_catalog_entry.h"
#include "testing/gtest/include/gtest/gtest.h"

TEST(AdBlockRegionalServiceTest, UserModelLanguages) {
  std::vector<hns_shields::FilterListCatalogEntry> catalog;
  catalog.push_back(hns_shields::FilterListCatalogEntry(
      "uuid", "https://hns.com", "Testing Filter List #1", {"fr"},
      "https://support.hns.com", "componentid", "base64publickey",
      "Filter list for testing purposes"));
  catalog.push_back(hns_shields::FilterListCatalogEntry(
      "uuid", "https://hns.com", "Testing Filter List #2", {"en"},
      "https://support.hns.com", "componentid", "base64publickey",
      "Filter list for testing purposes"));
  catalog.push_back(hns_shields::FilterListCatalogEntry(
      "uuid", "https://hns.com", "Testing Filter List #2", {"fr"},
      "https://support.hns.com", "componentid", "base64publickey",
      "Filter list for testing purposes"));

  std::vector<std::string> languages({ "fr", "fR", "fr-FR", "fr-ca" });
  std::for_each(
      languages.begin(), languages.end(), [&](const std::string& language) {
        EXPECT_EQ(
            hns_shields::FindAdBlockFilterListsByLocale(catalog, language)
                .size(),
            2UL);
      });

  EXPECT_EQ(hns_shields::FindAdBlockFilterListsByLocale(catalog, "en").size(),
            1UL);

  EXPECT_EQ(hns_shields::FindAdBlockFilterListsByLocale(catalog, "is").size(),
            0UL);
}
