/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_WEBCOMPAT_REPORTER_BROWSER_FIELDS_H_
#define HNS_COMPONENTS_WEBCOMPAT_REPORTER_BROWSER_FIELDS_H_

#include "hns/components/hns_shields/common/hns_shields_panel.mojom.h"

namespace webcompat_reporter {

extern const char kFPBlockSettingField[];
extern const char kAdBlockSettingField[];
extern const char kAdBlockListsField[];
extern const char kShieldsEnabledField[];
extern const char kLanguagesField[];
extern const char kLanguageFarblingField[];
extern const char kHnsVPNEnabledField[];

extern const char kSiteURLField[];
extern const char kDetailsField[];
extern const char kContactField[];
extern const char kDomainField[];
extern const char kApiKeyField[];

const char* GetAdBlockModeString(
    hns_shields::mojom::AdBlockMode ad_block_mode);
const char* GetFingerprintModeString(
    hns_shields::mojom::FingerprintMode fp_block_mode);

}  // namespace webcompat_reporter

#endif  // HNS_COMPONENTS_WEBCOMPAT_REPORTER_BROWSER_FIELDS_H_
