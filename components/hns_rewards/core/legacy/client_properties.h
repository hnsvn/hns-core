/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_CLIENT_PROPERTIES_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_CLIENT_PROPERTIES_H_

#include <stdint.h>
#include <map>
#include <string>

#include "base/values.h"
#include "hns/components/hns_rewards/core/legacy/wallet_info_properties.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"

namespace hns_rewards::internal {

struct ClientProperties {
  ClientProperties();
  ClientProperties(ClientProperties&& other);
  ClientProperties& operator=(ClientProperties&& other);
  ~ClientProperties();

  bool operator==(const ClientProperties& rhs) const;

  bool operator!=(const ClientProperties& rhs) const;

  base::Value::Dict ToValue() const;
  bool FromValue(const base::Value::Dict& value);

  std::string ToJson() const;
  bool FromJson(const std::string& json);

  WalletInfoProperties wallet_info;
  uint64_t boot_timestamp;
  uint64_t reconcile_timestamp;
  double fee_amount;
  bool user_changed_fee;
  bool auto_contribute;
  bool rewards_enabled;
  std::map<std::string, bool> inline_tips;
};

}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_CLIENT_PROPERTIES_H_
