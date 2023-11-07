/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_CONFIRMATIONS_LEGACY_CONFIRMATION_MIGRATION_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_CONFIRMATIONS_LEGACY_CONFIRMATION_MIGRATION_H_

#include "hns/components/hns_ads/core/public/ads_callback.h"

namespace hns_ads {

void MigrateConfirmationState(InitializeCallback callback);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_CONFIRMATIONS_LEGACY_CONFIRMATION_MIGRATION_H_
