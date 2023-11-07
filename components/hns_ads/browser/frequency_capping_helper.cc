/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/browser/frequency_capping_helper.h"

#include "base/no_destructor.h"
#include "base/time/time.h"

namespace hns_ads {

FrequencyCappingHelper::FrequencyCappingHelper() = default;

FrequencyCappingHelper::~FrequencyCappingHelper() = default;

FrequencyCappingHelper* FrequencyCappingHelper::GetInstance() {
  static base::NoDestructor<FrequencyCappingHelper> instance;
  return instance.get();
}

void FrequencyCappingHelper::RecordAdEventForId(
    const std::string& id,
    const std::string& ad_type,
    const std::string& confirmation_type,
    const base::Time time) {
  history_.RecordForId(id, ad_type, confirmation_type, time);
}

std::vector<base::Time> FrequencyCappingHelper::GetAdEventHistory(
    const std::string& ad_type,
    const std::string& confirmation_type) const {
  return history_.Get(ad_type, confirmation_type);
}

void FrequencyCappingHelper::ResetAdEventHistoryForId(const std::string& id) {
  history_.ResetForId(id);
}

}  // namespace hns_ads
