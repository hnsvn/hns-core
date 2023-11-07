/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_TIME_TIME_FORMATTING_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_TIME_TIME_FORMATTING_UTIL_H_

#include <string>

namespace base {
class Time;
}  // namespace base

namespace hns_ads {

std::string LongFriendlyDateAndTime(base::Time time,
                                    bool use_sentence_style = true);

std::string FriendlyDateAndTime(base::Time time,
                                bool use_sentence_style = true);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_TIME_TIME_FORMATTING_UTIL_H_
