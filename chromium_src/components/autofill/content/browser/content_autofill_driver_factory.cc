// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "components/autofill/content/browser/content_autofill_driver_factory.h"

#include "hns/components/constants/pref_names.h"
#include "components/autofill/core/browser/browser_autofill_manager.h"

namespace autofill {

class HnsBrowserAutofillManager : public BrowserAutofillManager {
 public:
  using BrowserAutofillManager::BrowserAutofillManager;

  bool IsAutofillEnabled() const override {
    auto enabled = BrowserAutofillManager::IsAutofillEnabled();
    if (client().GetProfileType() !=
            profile_metrics::BrowserProfileType::kIncognito &&
        client().GetProfileType() !=
            profile_metrics::BrowserProfileType::kOtherOffTheRecordProfile) {
      return enabled;
    }
    enabled = enabled &&
              client().GetPrefs()->GetBoolean(kHnsAutofillPrivateWindows);
    return enabled;
  }
};

}  // namespace autofill

#define BrowserAutofillManager HnsBrowserAutofillManager
#include "src/components/autofill/content/browser/content_autofill_driver_factory.cc"
#undef BrowserAutofillManager
