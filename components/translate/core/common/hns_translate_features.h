/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_TRANSLATE_CORE_COMMON_HNS_TRANSLATE_FEATURES_H_
#define HNS_COMPONENTS_TRANSLATE_CORE_COMMON_HNS_TRANSLATE_FEATURES_H_

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace translate {

namespace features {
BASE_DECLARE_FEATURE(kUseHnsTranslateGo);

extern const base::FeatureParam<bool> kUpdateLanguageListParam;
extern const base::FeatureParam<bool> kReplaceSecurityOriginParam;

BASE_DECLARE_FEATURE(kHnsEnableAutoTranslate);

}  // namespace features

// The translate engine can work the folowing ways:
// 1. IsHnsTranslateGoAvailable() == true: The internal translation engine
//    is used to translate the pages.
// 2. IsHnsTranslateGoAvailable() == false: no translation is available.
//    Chromium Translate feature should also be disabled.
bool IsHnsTranslateGoAvailable();

// True if the supported language list can be updated from the backend. False by
// default.
bool ShouldUpdateLanguagesList();

// True if the actual translate requests in the scripts are redirected to the
// google translate endpoint. False by default, use it only for local testing.
bool UseGoogleTranslateEndpoint();

// True if automatic translation logic is enabled.
// Includes core logic and UI elements.
bool IsHnsAutoTranslateEnabled();

}  // namespace translate

#endif  // HNS_COMPONENTS_TRANSLATE_CORE_COMMON_HNS_TRANSLATE_FEATURES_H_
