/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/browser/browser_util.h"

#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/version_info/version_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

std::string GetBrowserVersionNumber() {
  return version_info::GetHnsChromiumVersionNumber();
}

bool WasBrowserUpgraded() {
  static absl::optional<bool> was_upgraded;

  if (was_upgraded) {
    return *was_upgraded;
  }

  const std::string version_number = GetBrowserVersionNumber();

  const std::string last_version_number =
      AdsClientHelper::GetInstance()->GetStringPref(
          prefs::kBrowserVersionNumber);

  was_upgraded = version_number != last_version_number;

  if (was_upgraded) {
    AdsClientHelper::GetInstance()->SetStringPref(prefs::kBrowserVersionNumber,
                                                  version_number);
  }

  return *was_upgraded;
}

}  // namespace hns_ads
