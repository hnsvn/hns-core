// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stddef.h>

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "base/command_line.h"
#include "base/files/scoped_temp_dir.h"
#include "base/stl_util.h"
#include "base/values.h"
#include "hns/components/search_engines/hns_prepopulated_engines.h"
#include "components/google/core/common/google_switches.h"
#include "components/search_engines/search_engines_pref_names.h"
#include "components/search_engines/search_terms_data.h"
#include "components/search_engines/template_url.h"
#include "components/search_engines/template_url_data_util.h"
#include "components/search_engines/template_url_prepopulate_data.h"
#include "components/search_engines/template_url_service.h"
#include "components/search_engines/testing_search_terms_data.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {

const char kCountryIDAtInstall[] = "countryid_at_install";

std::string GetHostFromTemplateURLData(const TemplateURLData& data) {
  return TemplateURL(data).url_ref().GetHost(SearchTermsData());
}

using namespace TemplateURLPrepopulateData;  // NOLINT

const PrepopulatedEngine* const kHnsAddedEngines[] = {
    &startpage,
};

const std::unordered_set<std::wstring> kOverriddenEnginesNames = {L"DuckDuckGo",
                                                                  L"Qwant"};

std::vector<const PrepopulatedEngine*> GetAllPrepopulatedEngines() {
  std::vector<const PrepopulatedEngine*> engines =
      TemplateURLPrepopulateData::GetAllPrepopulatedEngines();
  engines.insert(engines.end(), std::begin(kHnsAddedEngines),
                 std::end(kHnsAddedEngines));
  return engines;
}

}  // namespace

class HnsTemplateURLPrepopulateDataTest : public testing::Test {
 public:
  void SetUp() override {
    TemplateURLPrepopulateData::RegisterProfilePrefs(prefs_.registry());
    // Real registration happens in `hns/browser/hns_profile_prefs.cc`
    // Calling hns::RegisterProfilePrefs() causes some problems though
    auto* registry = prefs_.registry();
    registry->RegisterIntegerPref(
        prefs::kHnsDefaultSearchVersion,
        TemplateURLPrepopulateData::kHnsCurrentDataVersion);
  }

  void CheckForCountry(char digit1, char digit2, int prepopulate_id) {
    prefs_.SetInteger(kCountryIDAtInstall, digit1 << 8 | digit2);
    prefs_.SetInteger(prefs::kHnsDefaultSearchVersion,
                      TemplateURLPrepopulateData::kHnsCurrentDataVersion);
    size_t default_index;
    std::vector<std::unique_ptr<TemplateURLData>> t_urls =
        TemplateURLPrepopulateData::GetPrepopulatedEngines(&prefs_,
                                                           &default_index);
    EXPECT_EQ(prepopulate_id, t_urls[default_index]->prepopulate_id);
  }

 protected:
  sync_preferences::TestingPrefServiceSyncable prefs_;
};

// Verifies that the set of all prepopulate data doesn't contain entries with
// duplicate keywords. This should make us notice if Chromium adds a search
// engine in the future that Hns already added.
TEST_F(HnsTemplateURLPrepopulateDataTest, UniqueKeywords) {
  using PrepopulatedEngine = TemplateURLPrepopulateData::PrepopulatedEngine;
  const std::vector<const PrepopulatedEngine*> all_engines =
      ::GetAllPrepopulatedEngines();
  std::set<std::wstring> unique_keywords;
  for (const PrepopulatedEngine* engine : all_engines) {
    ASSERT_TRUE(unique_keywords.find(engine->keyword) == unique_keywords.end());
    unique_keywords.insert(engine->keyword);
  }
}

// Verifies that engines we override are used and not the original engines.
TEST_F(HnsTemplateURLPrepopulateDataTest, OverriddenEngines) {
  using PrepopulatedEngine = TemplateURLPrepopulateData::PrepopulatedEngine;
  const std::vector<const PrepopulatedEngine*> all_engines =
      ::GetAllPrepopulatedEngines();
  for (const PrepopulatedEngine* engine : all_engines) {
    if (kOverriddenEnginesNames.count(engine->name) > 0)
      ASSERT_GE(static_cast<unsigned int>(engine->id),
                TemplateURLPrepopulateData::HNS_PREPOPULATED_ENGINES_START);
  }
}

// Verifies that the set of prepopulate data for each locale
// doesn't contain entries with duplicate ids.
TEST_F(HnsTemplateURLPrepopulateDataTest, UniqueIDs) {
  const int kCountryIds[] = {'D' << 8 | 'E', 'F' << 8 | 'R', 'U' << 8 | 'S',
                             -1};

  for (int country_id : kCountryIds) {
    prefs_.SetInteger(kCountryIDAtInstall, country_id);
    std::vector<std::unique_ptr<TemplateURLData>> urls =
        GetPrepopulatedEngines(&prefs_, nullptr);
    std::set<int> unique_ids;
    for (auto& url : urls) {
      ASSERT_TRUE(unique_ids.find(url->prepopulate_id) == unique_ids.end());
      unique_ids.insert(url->prepopulate_id);
    }
  }
}

// Verifies that each prepopulate data entry has required fields
TEST_F(HnsTemplateURLPrepopulateDataTest, ProvidersFromPrepopulated) {
  size_t default_index;
  std::vector<std::unique_ptr<TemplateURLData>> t_urls =
      TemplateURLPrepopulateData::GetPrepopulatedEngines(&prefs_,
                                                         &default_index);

  // Ensure all the URLs have the required fields populated.
  ASSERT_FALSE(t_urls.empty());
  for (size_t i = 0; i < t_urls.size(); ++i) {
    ASSERT_FALSE(t_urls[i]->short_name().empty());
    ASSERT_FALSE(t_urls[i]->keyword().empty());
    ASSERT_FALSE(t_urls[i]->favicon_url.host().empty());
    ASSERT_FALSE(GetHostFromTemplateURLData(*t_urls[i]).empty());
    ASSERT_FALSE(t_urls[i]->input_encodings.empty());
    EXPECT_GT(t_urls[i]->prepopulate_id, 0);
    EXPECT_TRUE(t_urls[0]->safe_for_autoreplace);
    EXPECT_TRUE(t_urls[0]->date_created.is_null());
    EXPECT_TRUE(t_urls[0]->last_modified.is_null());
  }
}

// Verifies default search provider for locale
TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForArgentina) {
  CheckForCountry('A', 'R', PREPOPULATED_ENGINE_ID_HNS);
}

TEST_F(HnsTemplateURLPrepopulateDataTest, DefaultSearchProvidersForIndia) {
  CheckForCountry('I', 'N', PREPOPULATED_ENGINE_ID_HNS);
}

TEST_F(HnsTemplateURLPrepopulateDataTest, DefaultSearchProvidersForBrazil) {
  CheckForCountry('B', 'R', PREPOPULATED_ENGINE_ID_HNS);
}

TEST_F(HnsTemplateURLPrepopulateDataTest, DefaultSearchProvidersForUSA) {
  CheckForCountry('U', 'S', PREPOPULATED_ENGINE_ID_HNS);
}

TEST_F(HnsTemplateURLPrepopulateDataTest, DefaultSearchProvidersForGermany) {
  CheckForCountry('D', 'E', PREPOPULATED_ENGINE_ID_HNS);
}

TEST_F(HnsTemplateURLPrepopulateDataTest, DefaultSearchProvidersForFrance) {
  CheckForCountry('F', 'R', PREPOPULATED_ENGINE_ID_HNS);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForGreatBritain) {
  CheckForCountry('G', 'B', PREPOPULATED_ENGINE_ID_HNS);
}

TEST_F(HnsTemplateURLPrepopulateDataTest, DefaultSearchProvidersForCanada) {
  CheckForCountry('C', 'A', PREPOPULATED_ENGINE_ID_HNS);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForAustralia) {
  CheckForCountry('A', 'U', PREPOPULATED_ENGINE_ID_GOOGLE);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForNewZealand) {
  CheckForCountry('N', 'Z', PREPOPULATED_ENGINE_ID_GOOGLE);
}

TEST_F(HnsTemplateURLPrepopulateDataTest, DefaultSearchProvidersForIreland) {
  CheckForCountry('I', 'E', PREPOPULATED_ENGINE_ID_GOOGLE);
}

TEST_F(HnsTemplateURLPrepopulateDataTest, DefaultSearchProvidersForAustria) {
  CheckForCountry('A', 'T', PREPOPULATED_ENGINE_ID_HNS);
}

TEST_F(HnsTemplateURLPrepopulateDataTest, DefaultSearchProvidersForSpain) {
  CheckForCountry('E', 'S', PREPOPULATED_ENGINE_ID_HNS);
}

TEST_F(HnsTemplateURLPrepopulateDataTest, DefaultSearchProvidersForMexico) {
  CheckForCountry('M', 'X', PREPOPULATED_ENGINE_ID_HNS);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForRepublicOfArmenia) {
  CheckForCountry('A', 'M', PREPOPULATED_ENGINE_ID_YANDEX);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForRepublicOfAzerbaijan) {
  CheckForCountry('A', 'Z', PREPOPULATED_ENGINE_ID_YANDEX);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForRepublicOfBelarus) {
  CheckForCountry('B', 'Y', PREPOPULATED_ENGINE_ID_YANDEX);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForKyrgyzRepublic) {
  CheckForCountry('K', 'G', PREPOPULATED_ENGINE_ID_YANDEX);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForRepublicOfKazakhstan) {
  CheckForCountry('K', 'Z', PREPOPULATED_ENGINE_ID_YANDEX);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForRepublicOfMoldova) {
  CheckForCountry('M', 'D', PREPOPULATED_ENGINE_ID_YANDEX);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForRussianFederation) {
  CheckForCountry('R', 'U', PREPOPULATED_ENGINE_ID_YANDEX);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForRepublicOfTajikistan) {
  CheckForCountry('T', 'J', PREPOPULATED_ENGINE_ID_YANDEX);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForTurkmenistan) {
  CheckForCountry('T', 'M', PREPOPULATED_ENGINE_ID_YANDEX);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForRepublicOfUzbekistan) {
  CheckForCountry('U', 'Z', PREPOPULATED_ENGINE_ID_YANDEX);
}

TEST_F(HnsTemplateURLPrepopulateDataTest,
       DefaultSearchProvidersForSouthKorea) {
  CheckForCountry('K', 'R', PREPOPULATED_ENGINE_ID_NAVER);
}
