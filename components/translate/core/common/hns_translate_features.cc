/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/translate/core/common/hns_translate_features.h"

#include "base/command_line.h"
#include "hns/components/translate/core/common/hns_translate_switches.h"

namespace translate {

namespace features {
BASE_FEATURE(kUseHnsTranslateGo,
             "UseHnsTranslateGo",
             base::FeatureState::FEATURE_ENABLED_BY_DEFAULT);

const base::FeatureParam<bool> kUpdateLanguageListParam{
    &kUseHnsTranslateGo, "update-languages", false};

BASE_FEATURE(kHnsEnableAutoTranslate,
             "HnsEnableAutoTranslate",
             base::FeatureState::FEATURE_DISABLED_BY_DEFAULT);
}  // namespace features

bool IsHnsTranslateGoAvailable() {
  return base::FeatureList::IsEnabled(features::kUseHnsTranslateGo);
}

bool ShouldUpdateLanguagesList() {
  return IsHnsTranslateGoAvailable() &&
         features::kUpdateLanguageListParam.Get();
}

bool UseGoogleTranslateEndpoint() {
  return IsHnsTranslateGoAvailable() &&
         base::CommandLine::ForCurrentProcess()->HasSwitch(
             switches::kHnsTranslateUseGoogleEndpoint);
}

bool IsHnsAutoTranslateEnabled() {
  return base::FeatureList::IsEnabled(features::kHnsEnableAutoTranslate);
}

}  // namespace translate
