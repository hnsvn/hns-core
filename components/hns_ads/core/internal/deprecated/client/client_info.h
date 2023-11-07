/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DEPRECATED_CLIENT_CLIENT_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DEPRECATED_CLIENT_CLIENT_INFO_H_

#include <map>
#include <string>

#include "base/containers/flat_map.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/preferences/ad_preferences_info.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/resource/purchase_intent_signal_history_info.h"
#include "hns/components/hns_ads/core/internal/targeting/contextual/text_classification/model/text_classification_alias.h"
#include "hns/components/hns_ads/core/public/history/history_item_info.h"

namespace hns_ads {

struct ClientInfo final {
  ClientInfo();

  ClientInfo(const ClientInfo&);
  ClientInfo& operator=(const ClientInfo&);

  ClientInfo(ClientInfo&&) noexcept;
  ClientInfo& operator=(ClientInfo&&) noexcept;

  ~ClientInfo();

  base::Value::Dict ToValue() const;
  [[nodiscard]] bool FromValue(const base::Value::Dict& dict);

  std::string ToJson() const;
  [[nodiscard]] bool FromJson(const std::string& json);

  AdPreferencesInfo ad_preferences;
  HistoryItemList history_items;
  base::flat_map<
      /*ad_type*/ std::string,
      std::map</*creative_instance_id*/ std::string, /*seen_ad*/ bool>>
      seen_ads;
  base::flat_map<
      /*ad_type*/ std::string,
      std::map</*advertiser_id*/ std::string, /*seen_advertiser*/ bool>>
      seen_advertisers;
  TextClassificationProbabilityList text_classification_probabilities;
  PurchaseIntentSignalHistoryMap purchase_intent_signal_history;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DEPRECATED_CLIENT_CLIENT_INFO_H_
