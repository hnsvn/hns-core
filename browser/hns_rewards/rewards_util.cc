/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_rewards/rewards_util.h"

#include "hns/browser/profiles/profile_util.h"
#include "hns/components/hns_rewards/common/rewards_util.h"
#include "components/prefs/pref_service.h"

namespace hns_rewards {

bool IsSupportedForProfile(Profile* profile, IsSupportedOptions options) {
  DCHECK(profile);
  return hns::IsRegularProfile(profile) &&
         IsSupported(profile->GetPrefs(), options);
}

}  // namespace hns_rewards
