/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_SUBSCRIPTION_SERVICE_MANAGER_OBSERVER_H_
#define HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_SUBSCRIPTION_SERVICE_MANAGER_OBSERVER_H_

#include "base/observer_list_types.h"

namespace hns_shields {

class AdBlockSubscriptionServiceManagerObserver : public base::CheckedObserver {
 public:
  ~AdBlockSubscriptionServiceManagerObserver() override = default;
  virtual void OnServiceUpdateEvent() {}
};

}  // namespace hns_shields

#endif  // HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_SUBSCRIPTION_SERVICE_MANAGER_OBSERVER_H_
