/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_BROWSER_ADS_P2A_H_
#define HNS_COMPONENTS_HNS_ADS_BROWSER_ADS_P2A_H_

#include <string>

class PrefService;
class PrefRegistrySimple;

namespace hns_ads {

void RegisterP2APrefs(PrefRegistrySimple* registry);

void RecordInWeeklyStorageAndEmitP2AHistogramName(PrefService* prefs,
                                                  const std::string& name);

void SuspendP2AHistograms();

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_BROWSER_ADS_P2A_H_
