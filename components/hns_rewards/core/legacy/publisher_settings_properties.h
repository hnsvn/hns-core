/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_PUBLISHER_SETTINGS_PROPERTIES_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_PUBLISHER_SETTINGS_PROPERTIES_H_

#include <stdint.h>
#include <map>
#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/legacy/report_balance_properties.h"

namespace hns_rewards::internal {

struct PublisherSettingsProperties {
  PublisherSettingsProperties();
  PublisherSettingsProperties(PublisherSettingsProperties&& other);
  PublisherSettingsProperties& operator=(PublisherSettingsProperties&& other);
  ~PublisherSettingsProperties();

  bool operator==(const PublisherSettingsProperties& rhs) const;

  bool operator!=(const PublisherSettingsProperties& rhs) const;

  base::Value::Dict ToValue() const;
  bool FromValue(const base::Value::Dict& value);

  std::string ToJson() const;
  bool FromJson(const std::string& json);

  uint64_t min_page_time_before_logging_a_visit;
  uint32_t min_visits_for_publisher_relevancy;
  bool allow_non_verified_sites_in_list;
  std::map<std::string, ReportBalanceProperties> monthly_balances;
  std::vector<std::string> processed_pending_publishers;
};

}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_PUBLISHER_SETTINGS_PROPERTIES_H_
