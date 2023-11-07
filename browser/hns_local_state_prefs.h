/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_LOCAL_STATE_PREFS_H_
#define HNS_BROWSER_HNS_LOCAL_STATE_PREFS_H_

class PrefRegistrySimple;

namespace hns {

void RegisterLocalStatePrefs(PrefRegistrySimple* registry);

}  // namespace hns

#endif  // HNS_BROWSER_HNS_LOCAL_STATE_PREFS_H_
