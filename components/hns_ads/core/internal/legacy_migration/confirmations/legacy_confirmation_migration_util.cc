/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/legacy_migration/confirmations/legacy_confirmation_migration_util.h"

#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"

namespace hns_ads {

bool HasMigratedConfirmation() {
  return AdsClientHelper::GetInstance()->GetBooleanPref(
      prefs::kHasMigratedConfirmationState);
}

}  // namespace hns_ads
