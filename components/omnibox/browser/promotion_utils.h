/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_OMNIBOX_BROWSER_PROMOTION_UTILS_H_
#define HNS_COMPONENTS_OMNIBOX_BROWSER_PROMOTION_UTILS_H_

#include <string>

class AutocompleteInput;
class AutocompleteResult;
struct AutocompleteMatch;

namespace hns_search_conversion {
enum class ConversionType;
}  // namespace hns_search_conversion

// Exposed for testing.
void SortHnsSearchPromotionMatch(AutocompleteResult* result);

// True when |match| is the hns search conversion promotion match for |input|.
bool IsHnsSearchPromotionMatch(const AutocompleteMatch& match);

hns_search_conversion::ConversionType GetConversionTypeFromMatch(
    const AutocompleteMatch& match);
void SetConversionTypeToMatch(hns_search_conversion::ConversionType type,
                              AutocompleteMatch* match);

#endif  // HNS_COMPONENTS_OMNIBOX_BROWSER_PROMOTION_UTILS_H_
