/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_DEPOSITS_DEPOSITS_FACTORY_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_DEPOSITS_DEPOSITS_FACTORY_H_

#include <memory>

#include "hns/components/hns_ads/core/internal/account/deposits/deposit_interface.h"

namespace hns_ads {

class ConfirmationType;

class DepositsFactory final {
 public:
  static std::unique_ptr<DepositInterface> Build(
      const ConfirmationType& confirmation_type);
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_DEPOSITS_DEPOSITS_FACTORY_H_
