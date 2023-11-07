/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_ADS_AD_EVENT_AD_EVENT_HISTORY_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_ADS_AD_EVENT_AD_EVENT_HISTORY_H_

#include <string>
#include <vector>

#include "base/containers/flat_map.h"
#include "hns/components/hns_ads/core/public/export.h"

namespace base {
class Time;
}  // namespace base

namespace hns_ads {

class ADS_EXPORT AdEventHistory final {
 public:
  AdEventHistory();

  AdEventHistory(const AdEventHistory&) = delete;
  AdEventHistory& operator=(const AdEventHistory&) = delete;

  AdEventHistory(AdEventHistory&&) noexcept = delete;
  AdEventHistory& operator=(AdEventHistory&&) noexcept = delete;

  ~AdEventHistory();

  void RecordForId(const std::string& id,
                   const std::string& ad_type,
                   const std::string& confirmation_type,
                   base::Time time);

  std::vector<base::Time> Get(const std::string& ad_type,
                              const std::string& confirmation_type) const;

  void ResetForId(const std::string& id);

 private:
  base::flat_map<std::string,
                 base::flat_map<std::string, std::vector<base::Time>>>
      history_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_ADS_AD_EVENT_AD_EVENT_HISTORY_H_
