/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/flags/environment/environment_flag_util.h"

#include "hns/components/hns_ads/core/internal/global_state/global_state.h"

namespace hns_ads {

bool IsProductionEnvironment() {
  return GlobalState::GetInstance()->Flags().environment_type ==
         mojom::EnvironmentType::kProduction;
}

}  // namespace hns_ads
