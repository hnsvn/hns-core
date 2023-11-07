/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/search_engines/template_url_prepopulate_data.h"

#include <map>
#include <vector>

#include "base/containers/fixed_flat_map.h"
#include "base/containers/flat_map.h"
#include "base/no_destructor.h"
#include "base/stl_util.h"
#include "base/strings/utf_string_conversions.h"
#include "hns/components/search_engines/hns_prepopulated_engines.h"
#include "build/build_config.h"
#include "components/country_codes/country_codes.h"
#include "components/search_engines/search_engines_pref_names.h"

#define GetDataVersion GetDataVersion_ChromiumImpl
#if BUILDFLAG(IS_ANDROID)
#define GetLocalPrepopulatedEngines GetLocalPrepopulatedEngines_Unused
#endif
#define GetPrepopulatedDefaultSearch GetPrepopulatedDefaultSearch_Unused
#define GetPrepopulatedEngine GetPrepopulatedEngine_Unused
#define GetPrepopulatedEngines GetPrepopulatedEngines_Unused
#include "src/components/search_engines/template_url_prepopulate_data.cc"
#undef GetDataVersion
#if BUILDFLAG(IS_ANDROID)
#undef GetLocalPrepopulatedEngines
#endif
#undef GetPrepopulatedDefaultSearch
#undef GetPrepopulatedEngine
#undef GetPrepopulatedEngines

namespace TemplateURLPrepopulateData {

namespace {

// Default order in which engines will appear in the UI.
constexpr HnsPrepopulatedEngineID kHnsEnginesDefault[] = {
    PREPOPULATED_ENGINE_ID_HNS,      PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO, PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,       PREPOPULATED_ENGINE_ID_STARTPAGE,
};

// Variations of the order / default options by country.
constexpr HnsPrepopulatedEngineID kHnsEnginesWithEcosia[] = {
    PREPOPULATED_ENGINE_ID_HNS,      PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO, PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,       PREPOPULATED_ENGINE_ID_STARTPAGE,
    PREPOPULATED_ENGINE_ID_ECOSIA,
};

constexpr HnsPrepopulatedEngineID kHnsEnginesWithYandex[] = {
    PREPOPULATED_ENGINE_ID_YANDEX,    PREPOPULATED_ENGINE_ID_HNS,
    PREPOPULATED_ENGINE_ID_GOOGLE,    PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
    PREPOPULATED_ENGINE_ID_QWANT,     PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
};

constexpr HnsPrepopulatedEngineID kHnsEnginesDE[] = {
    PREPOPULATED_ENGINE_ID_HNS,  PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE,
    PREPOPULATED_ENGINE_ID_QWANT,  PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_BING,   PREPOPULATED_ENGINE_ID_STARTPAGE,
    PREPOPULATED_ENGINE_ID_ECOSIA,
};

constexpr HnsPrepopulatedEngineID kHnsEnginesFR[] = {
    PREPOPULATED_ENGINE_ID_HNS,  PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_GOOGLE, PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
    PREPOPULATED_ENGINE_ID_BING,   PREPOPULATED_ENGINE_ID_STARTPAGE,
    PREPOPULATED_ENGINE_ID_ECOSIA,
};

constexpr HnsPrepopulatedEngineID kHnsEnginesAUIE[] = {
    PREPOPULATED_ENGINE_ID_HNS,  PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE,
    PREPOPULATED_ENGINE_ID_GOOGLE, PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,   PREPOPULATED_ENGINE_ID_STARTPAGE,
    PREPOPULATED_ENGINE_ID_ECOSIA,
};

constexpr HnsPrepopulatedEngineID kHnsEnginesKR[] = {
    PREPOPULATED_ENGINE_ID_HNS,
    PREPOPULATED_ENGINE_ID_NAVER,
    PREPOPULATED_ENGINE_ID_DAUM,
    PREPOPULATED_ENGINE_ID_GOOGLE,
};

constexpr HnsPrepopulatedEngineID kHnsEnginesNZ[] = {
    PREPOPULATED_ENGINE_ID_HNS,  PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE,
    PREPOPULATED_ENGINE_ID_GOOGLE, PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,   PREPOPULATED_ENGINE_ID_STARTPAGE,
};

// A map to keep track of a full list of default engines for countries
// that don't use the default list.
constexpr auto kDefaultEnginesByCountryIdMap =
    base::MakeFixedFlatMap<int, base::span<const HnsPrepopulatedEngineID>>(
        {{country_codes::CountryCharsToCountryID('A', 'M'),
          kHnsEnginesWithYandex},
         {country_codes::CountryCharsToCountryID('A', 'T'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('A', 'U'), kHnsEnginesAUIE},
         {country_codes::CountryCharsToCountryID('A', 'Z'),
          kHnsEnginesWithYandex},
         {country_codes::CountryCharsToCountryID('B', 'E'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('B', 'Y'),
          kHnsEnginesWithYandex},
         {country_codes::CountryCharsToCountryID('C', 'A'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('C', 'H'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('D', 'E'), kHnsEnginesDE},
         {country_codes::CountryCharsToCountryID('D', 'K'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('E', 'S'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('F', 'I'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('F', 'R'), kHnsEnginesFR},
         {country_codes::CountryCharsToCountryID('G', 'B'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('G', 'R'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('H', 'U'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('I', 'E'), kHnsEnginesAUIE},
         {country_codes::CountryCharsToCountryID('I', 'T'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('K', 'G'),
          kHnsEnginesWithYandex},
         {country_codes::CountryCharsToCountryID('K', 'R'), kHnsEnginesKR},
         {country_codes::CountryCharsToCountryID('K', 'Z'),
          kHnsEnginesWithYandex},
         {country_codes::CountryCharsToCountryID('L', 'U'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('M', 'D'),
          kHnsEnginesWithYandex},
         {country_codes::CountryCharsToCountryID('N', 'L'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('N', 'O'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('N', 'Z'), kHnsEnginesNZ},
         {country_codes::CountryCharsToCountryID('P', 'T'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('R', 'U'),
          kHnsEnginesWithYandex},
         {country_codes::CountryCharsToCountryID('S', 'E'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('T', 'J'),
          kHnsEnginesWithYandex},
         {country_codes::CountryCharsToCountryID('T', 'M'),
          kHnsEnginesWithYandex},
         {country_codes::CountryCharsToCountryID('U', 'S'),
          kHnsEnginesWithEcosia},
         {country_codes::CountryCharsToCountryID('U', 'Z'),
          kHnsEnginesWithYandex}});

// A versioned map tracking the singular default search engine per-country.
HnsPrepopulatedEngineID GetDefaultSearchEngine(int country_id, int version) {
  const HnsPrepopulatedEngineID default_v6 = PREPOPULATED_ENGINE_ID_GOOGLE;
  static constexpr auto kContentV6 =
      base::MakeFixedFlatMap<int, HnsPrepopulatedEngineID>({
          {country_codes::CountryCharsToCountryID('A', 'U'),
           PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
          {country_codes::CountryCharsToCountryID('F', 'R'),
           PREPOPULATED_ENGINE_ID_QWANT},
          {country_codes::CountryCharsToCountryID('D', 'E'),
           PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE},
          {country_codes::CountryCharsToCountryID('I', 'E'),
           PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
          {country_codes::CountryCharsToCountryID('N', 'Z'),
           PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
      });
  static constexpr auto kContentV8 =
      base::MakeFixedFlatMap<int, HnsPrepopulatedEngineID>({
          {country_codes::CountryCharsToCountryID('A', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('A', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('A', 'U'),
           PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
          {country_codes::CountryCharsToCountryID('B', 'Y'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('D', 'E'),
           PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE},
          {country_codes::CountryCharsToCountryID('F', 'R'),
           PREPOPULATED_ENGINE_ID_QWANT},
          {country_codes::CountryCharsToCountryID('I', 'E'),
           PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
          {country_codes::CountryCharsToCountryID('K', 'G'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('K', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('M', 'D'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('N', 'Z'),
           PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
          {country_codes::CountryCharsToCountryID('R', 'U'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'J'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('U', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
      });
  static constexpr auto kContentV16 =
      base::MakeFixedFlatMap<int, HnsPrepopulatedEngineID>({
          {country_codes::CountryCharsToCountryID('A', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('A', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('B', 'Y'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('F', 'R'),
           PREPOPULATED_ENGINE_ID_QWANT},
          {country_codes::CountryCharsToCountryID('K', 'G'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('K', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('M', 'D'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('R', 'U'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'J'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('U', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
      });
  static constexpr auto kContentV17 =
      base::MakeFixedFlatMap<int, HnsPrepopulatedEngineID>({
          {country_codes::CountryCharsToCountryID('A', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('A', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('B', 'Y'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('C', 'A'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('D', 'E'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('F', 'R'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('G', 'B'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('K', 'G'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('K', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('M', 'D'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('R', 'U'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'J'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('U', 'S'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('U', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
      });

  static constexpr auto kContentV20 =
      base::MakeFixedFlatMap<int, HnsPrepopulatedEngineID>({
          {country_codes::CountryCharsToCountryID('A', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('A', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('B', 'Y'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('C', 'A'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('D', 'E'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('F', 'R'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('G', 'B'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('K', 'G'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('K', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('M', 'D'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('R', 'U'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'J'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('U', 'S'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('U', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          // Added values in this version bellow
          {country_codes::CountryCharsToCountryID('A', 'T'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('E', 'S'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('M', 'X'),
           PREPOPULATED_ENGINE_ID_HNS},
      });

  static constexpr auto kContentV21 =
      base::MakeFixedFlatMap<int, HnsPrepopulatedEngineID>({
          {country_codes::CountryCharsToCountryID('A', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('A', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('B', 'Y'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('C', 'A'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('D', 'E'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('F', 'R'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('G', 'B'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('K', 'G'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('K', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('M', 'D'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('R', 'U'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'J'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('U', 'S'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('U', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('A', 'T'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('E', 'S'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('M', 'X'),
           PREPOPULATED_ENGINE_ID_HNS},
          // Added values in this version bellow
          {country_codes::CountryCharsToCountryID('A', 'R'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('B', 'R'),
           PREPOPULATED_ENGINE_ID_HNS},
      });
  static constexpr auto kContentV22 =
      base::MakeFixedFlatMap<int, HnsPrepopulatedEngineID>({
          {country_codes::CountryCharsToCountryID('A', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('A', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('B', 'Y'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('C', 'A'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('D', 'E'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('F', 'R'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('G', 'B'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('K', 'G'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('K', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('M', 'D'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('R', 'U'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'J'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('U', 'S'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('U', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('A', 'T'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('E', 'S'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('M', 'X'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('A', 'R'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('B', 'R'),
           PREPOPULATED_ENGINE_ID_HNS},
          // Added values in this version bellow
          {country_codes::CountryCharsToCountryID('I', 'N'),
           PREPOPULATED_ENGINE_ID_HNS},
      });
  static constexpr auto kContentV25 =
      base::MakeFixedFlatMap<int, HnsPrepopulatedEngineID>({
          {country_codes::CountryCharsToCountryID('A', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('A', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('B', 'Y'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('C', 'A'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('D', 'E'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('F', 'R'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('G', 'B'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('K', 'G'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('K', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('M', 'D'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('R', 'U'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'J'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('T', 'M'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('U', 'S'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('U', 'Z'),
           PREPOPULATED_ENGINE_ID_YANDEX},
          {country_codes::CountryCharsToCountryID('A', 'T'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('E', 'S'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('M', 'X'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('A', 'R'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('B', 'R'),
           PREPOPULATED_ENGINE_ID_HNS},
          {country_codes::CountryCharsToCountryID('I', 'N'),
           PREPOPULATED_ENGINE_ID_HNS},
          // Added values in this version bellow
          {country_codes::CountryCharsToCountryID('K', 'R'),
           PREPOPULATED_ENGINE_ID_NAVER},
      });

  if (version > 24) {
    auto* it = kContentV25.find(country_id);
    if (it == kContentV25.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 21) {
    auto* it = kContentV22.find(country_id);
    if (it == kContentV22.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 20) {
    auto* it = kContentV21.find(country_id);
    if (it == kContentV21.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 19) {
    auto* it = kContentV20.find(country_id);
    if (it == kContentV20.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 16) {
    auto* it = kContentV17.find(country_id);
    if (it == kContentV17.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 15) {
    auto* it = kContentV16.find(country_id);
    if (it == kContentV16.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 7) {
    auto* it = kContentV8.find(country_id);
    if (it == kContentV8.end()) {
      return default_v6;
    }
    return it->second;
  } else {
    auto* it = kContentV6.find(country_id);
    if (it == kContentV6.end()) {
      return default_v6;
    }
    return it->second;
  }
}

// Builds a vector of PrepulatedEngine objects from the given array of
// |engine_ids|. Fills in the default engine index for the given |country_id|,
// if asked.
std::vector<const PrepopulatedEngine*> GetEnginesFromEngineIDs(
    base::span<const HnsPrepopulatedEngineID> engine_ids,
    int country_id,
    HnsPrepopulatedEngineID default_engine_id,
    size_t* default_search_provider_index = nullptr) {
  std::vector<const PrepopulatedEngine*> engines;
  const auto& hns_engines_map =
      TemplateURLPrepopulateData::GetHnsEnginesMap();
  for (size_t i = 0; i < engine_ids.size(); ++i) {
    const PrepopulatedEngine* engine = hns_engines_map.at(engine_ids[i]);
    DCHECK(engine);
    if (engine) {
      engines.push_back(engine);
      if (default_search_provider_index && default_engine_id == engine_ids[i]) {
        *default_search_provider_index = i;
      }
    }
  }
  return engines;
}

void UpdateTemplateURLDataKeyword(
    const std::unique_ptr<TemplateURLData>& t_urld) {
  DCHECK(t_urld.get());
  switch (t_urld->prepopulate_id) {
    case PREPOPULATED_ENGINE_ID_GOOGLE:
      t_urld->SetKeyword(u":g");
      break;
    case PREPOPULATED_ENGINE_ID_BING:
      t_urld->SetKeyword(u":b");
      break;
  }
}

// Uses hns_engines_XX localized arrays of engine IDs instead of Chromium's
// localized arrays of PrepopulatedEngines to construct the vector of
// TemplateURLData. Also, fills in the default engine index for the given
// |country_id|.
std::vector<std::unique_ptr<TemplateURLData>>
GetHnsPrepopulatedEnginesForCountryID(
    int country_id,
    size_t* default_search_provider_index = nullptr,
    int version = kHnsCurrentDataVersion) {
  base::span<const HnsPrepopulatedEngineID> hns_engine_ids =
      kHnsEnginesDefault;

  // Check for a per-country override of this list
  const auto* it_country = kDefaultEnginesByCountryIdMap.find(country_id);
  if (it_country != kDefaultEnginesByCountryIdMap.end()) {
    hns_engine_ids = it_country->second;
  }
  DCHECK_GT(hns_engine_ids.size(), 0ul);

  // Get the default engine (overridable by country) for this version
  HnsPrepopulatedEngineID default_id =
      GetDefaultSearchEngine(country_id, version);

  // Build a vector PrepopulatedEngines from HnsPrepopulatedEngineIDs and
  // also get the default engine index
  std::vector<const PrepopulatedEngine*> engines = GetEnginesFromEngineIDs(
      hns_engine_ids, country_id, default_id, default_search_provider_index);
  DCHECK(engines.size() == hns_engine_ids.size());

  std::vector<std::unique_ptr<TemplateURLData>> t_urls;
  for (const PrepopulatedEngine* engine : engines) {
    std::unique_ptr<TemplateURLData> t_urld =
        TemplateURLDataFromPrepopulatedEngine(*engine);
    UpdateTemplateURLDataKeyword(t_urld);
    t_urls.push_back(std::move(t_urld));
  }

  return t_urls;
}

}  // namespace

// Redefines function with the same name in Chromium. We need to account for
// the version of Hns engines as well: kCurrentDataVersion is defined in
// prepopulated_engines.json and is bumped every time the json file is
// modified. Since we add our own engines we need to keep track of our
// version as well and combine it with Chromium's version.
int GetDataVersion(PrefService* prefs) {
  int data_version = GetDataVersion_ChromiumImpl(prefs);
  // Check if returned version was from preferences override and if so return
  // that version.
  if (prefs && prefs->HasPrefPath(prefs::kSearchProviderOverridesVersion))
    return data_version;
  return (data_version + kHnsCurrentDataVersion);
}

// Redefines function with the same name in Chromium. Modifies the function to
// get search engines defined by Hns.
std::vector<std::unique_ptr<TemplateURLData>> GetPrepopulatedEngines(
    PrefService* prefs,
    size_t* default_search_provider_index) {
  // If there is a set of search engines in the preferences file, it overrides
  // the built-in set.
  if (default_search_provider_index)
    *default_search_provider_index = 0;
  std::vector<std::unique_ptr<TemplateURLData>> t_urls =
      GetPrepopulatedTemplateURLData(prefs);
  if (!t_urls.empty())
    return t_urls;

  int version = kHnsCurrentDataVersion;
  if (prefs && prefs->HasPrefPath(prefs::kHnsDefaultSearchVersion)) {
    version = prefs->GetInteger(prefs::kHnsDefaultSearchVersion);
  }

  return GetHnsPrepopulatedEnginesForCountryID(
      country_codes::GetCountryIDFromPrefs(prefs),
      default_search_provider_index, version);
}

// Redefines function with the same name in Chromium. Modifies the function to
// get search engines defined by Hns.
#if BUILDFLAG(IS_ANDROID)

std::vector<std::unique_ptr<TemplateURLData>> GetLocalPrepopulatedEngines(
    const std::string& locale) {
  int country_id = country_codes::CountryStringToCountryID(locale);
  if (country_id == country_codes::kCountryIDUnknown) {
    LOG(ERROR) << "Unknown country code specified: " << locale;
    return std::vector<std::unique_ptr<TemplateURLData>>();
  }

  return GetHnsPrepopulatedEnginesForCountryID(country_id);
}

#endif

// Functions below are copied verbatim from
// components\search_engines\template_url_prepopulate_data.cc because they
// need to call our versions of redefined Chromium's functions.

std::unique_ptr<TemplateURLData> GetPrepopulatedEngine(PrefService* prefs,
                                                       int prepopulated_id) {
  size_t default_index;
  auto engines =
      TemplateURLPrepopulateData::GetPrepopulatedEngines(prefs, &default_index);
  for (auto& engine : engines) {
    if (engine->prepopulate_id == prepopulated_id)
      return std::move(engine);
  }
  return nullptr;
}

std::unique_ptr<TemplateURLData> GetPrepopulatedDefaultSearch(
    PrefService* prefs) {
  size_t default_search_index;
  // This could be more efficient.  We are loading all the URLs to only keep
  // the first one.
  std::vector<std::unique_ptr<TemplateURLData>> loaded_urls =
      GetPrepopulatedEngines(prefs, &default_search_index);

  return (default_search_index < loaded_urls.size())
             ? std::move(loaded_urls[default_search_index])
             : nullptr;
}

}  // namespace TemplateURLPrepopulateData
