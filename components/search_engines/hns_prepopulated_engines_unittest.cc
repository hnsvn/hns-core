/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/search_engines/hns_prepopulated_engines.h"
#include "components/search_engines/template_url_data.h"
#include "components/search_engines/template_url_prepopulate_data.h"
#include "testing/gtest/include/gtest/gtest.h"

TEST(HnsPrepopulatedEnginesTest, ModifiedProviderTest) {
  auto data = TemplateURLPrepopulateData::GetPrepopulatedEngine(
      nullptr, TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_BING);
  // Check modified bing provider url.
  EXPECT_EQ(data->url(), "https://www.bing.com/search?q={searchTerms}");
}
