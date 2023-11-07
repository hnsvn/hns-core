// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_SEARCH_CONVERSION_P3A_H_
#define HNS_COMPONENTS_HNS_SEARCH_CONVERSION_P3A_H_

#include "hns/components/hns_search_conversion/types.h"

class PrefRegistrySimple;
class PrefService;

namespace hns_search_conversion {
namespace p3a {

extern const char kSearchPromoButtonHistogramName[];
extern const char kSearchPromoBannerAHistogramName[];
extern const char kSearchPromoBannerBHistogramName[];
extern const char kSearchPromoBannerCHistogramName[];
extern const char kSearchPromoBannerDHistogramName[];
extern const char kSearchPromoNTPHistogramName[];

void RegisterLocalStatePrefs(PrefRegistrySimple* registry);
void RegisterLocalStatePrefsForMigration(PrefRegistrySimple* registry);
void MigrateObsoleteLocalStatePrefs(PrefService* local_state);
void RecordPromoShown(PrefService* prefs, ConversionType type);
void RecordPromoTrigger(PrefService* prefs, ConversionType type);
void RecordDefaultEngineChange(PrefService* prefs);

}  // namespace p3a
}  // namespace hns_search_conversion

#endif  // HNS_COMPONENTS_HNS_SEARCH_CONVERSION_P3A_H_
