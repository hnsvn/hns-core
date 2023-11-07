/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_FEDERATED_ELIGIBILITY_SERVICE_OBSERVER_H_
#define HNS_COMPONENTS_HNS_FEDERATED_ELIGIBILITY_SERVICE_OBSERVER_H_

#include "base/observer_list_types.h"

namespace hns_federated {

class Observer : public base::CheckedObserver {
 public:
  virtual void OnEligibilityChanged(bool is_eligible) = 0;

 protected:
  ~Observer() override = default;
};

}  // namespace hns_federated

#endif  // HNS_COMPONENTS_HNS_FEDERATED_ELIGIBILITY_SERVICE_OBSERVER_H_
