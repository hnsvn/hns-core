/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_private_new_tab_ui/common/pref_names.h"

#include "components/prefs/pref_registry_simple.h"

namespace hns_private_new_tab::prefs {

void RegisterProfilePrefs(PrefRegistrySimple* registry) {
  registry->RegisterBooleanPref(kHnsPrivateWindowDisclaimerDismissed, false);
  registry->RegisterBooleanPref(kHnsTorWindowDisclaimerDismissed, false);
}

}  // namespace hns_private_new_tab::prefs
