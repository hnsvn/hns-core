/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_LOCALE_COUNTRY_CODE_ANONYMITY_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_LOCALE_COUNTRY_CODE_ANONYMITY_UTIL_H_

#include <string>

namespace hns_ads {

// K-anonymity for country codes is a data privacy technique that involves
// modifying or suppressing data elements to ensure that the resulting dataset
// does not allow an individual to be identified based on their country of
// origin while preserving the overall statistical accuracy of the data.

// Return |true| if the given |country_code| is a member of the anonymity set,
// otherwise returns |false|.
bool IsCountryCodeMemberOfAnonymitySet(const std::string& country_code);

// Return |true| if the given |country_code| should be classified as other
// otherwise returns |false|.
bool ShouldClassifyCountryCodeAsOther(const std::string& country_code);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_LOCALE_COUNTRY_CODE_ANONYMITY_UTIL_H_
