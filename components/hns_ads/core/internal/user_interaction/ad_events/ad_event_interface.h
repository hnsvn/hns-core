/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_AD_EVENT_INTERFACE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_AD_EVENT_INTERFACE_H_

#include "hns/components/hns_ads/core/public/client/ads_client_callback.h"

namespace hns_ads {

template <typename T>
class AdEventInterface {
 public:
  virtual ~AdEventInterface() = default;

  virtual void FireEvent(const T&, ResultCallback callback) = 0;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_AD_EVENT_INTERFACE_H_
