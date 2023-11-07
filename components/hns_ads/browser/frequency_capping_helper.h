/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_BROWSER_FREQUENCY_CAPPING_HELPER_H_
#define HNS_COMPONENTS_HNS_ADS_BROWSER_FREQUENCY_CAPPING_HELPER_H_

#include <string>
#include <vector>

#include "hns/components/hns_ads/core/public/ads/ad_event/ad_event_history.h"

namespace base {
template <typename T>
class NoDestructor;
class Time;
}  // namespace base

namespace hns_ads {

class FrequencyCappingHelper {
 public:
  FrequencyCappingHelper(const FrequencyCappingHelper&) = delete;
  FrequencyCappingHelper& operator=(const FrequencyCappingHelper&) = delete;

  FrequencyCappingHelper(FrequencyCappingHelper&&) noexcept = delete;
  FrequencyCappingHelper& operator=(FrequencyCappingHelper&&) noexcept = delete;

  static FrequencyCappingHelper* GetInstance();

  void RecordAdEventForId(const std::string& id,
                          const std::string& ad_type,
                          const std::string& confirmation_type,
                          base::Time time);

  std::vector<base::Time> GetAdEventHistory(
      const std::string& ad_type,
      const std::string& confirmation_type) const;

  void ResetAdEventHistoryForId(const std::string& id);

 private:
  friend base::NoDestructor<FrequencyCappingHelper>;

  FrequencyCappingHelper();

  ~FrequencyCappingHelper();

  AdEventHistory history_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_BROWSER_FREQUENCY_CAPPING_HELPER_H_
