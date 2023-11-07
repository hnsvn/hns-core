/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/confirmations/user_data_builder/dynamic/confirmation_dynamic_user_data_builder.h"

#include <utility>

#include "base/functional/callback.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/internal/account/user_data/dynamic/diagnostic_id_user_data.h"
#include "hns/components/hns_ads/core/internal/account/user_data/dynamic/system_timestamp_user_data.h"

namespace hns_ads {

void BuildDynamicUserData(BuildUserDataCallback callback) {
  base::Value::Dict user_data;

  user_data.Merge(BuildDiagnosticIdUserData());
  user_data.Merge(BuildSystemTimestampUserData());

  std::move(callback).Run(std::move(user_data));
}

}  // namespace hns_ads
