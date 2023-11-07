/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_wallet/hns_wallet_context_utils.h"
#include "hns/browser/profiles/profile_util.h"
#include "hns/components/hns_wallet/common/common_utils.h"
#include "components/user_prefs/user_prefs.h"
#include "content/public/browser/browser_context.h"

namespace hns_wallet {

bool IsAllowedForContext(content::BrowserContext* context) {
  if (context && (!hns::IsRegularProfile(context) ||
                  !IsAllowed(user_prefs::UserPrefs::Get(context)))) {
    return false;
  }

  return true;
}

}  // namespace hns_wallet
