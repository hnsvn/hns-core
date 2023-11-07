/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/deposits/deposit_info.h"

namespace hns_ads {

bool DepositInfo::IsValid() const {
  return !creative_instance_id.empty();
}

}  // namespace hns_ads
