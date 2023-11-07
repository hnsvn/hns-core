/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_CORE_COMMON_FEATURES_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_CORE_COMMON_FEATURES_H_

#include "src/components/content_settings/core/common/features.h"  // IWYU pragma: export

namespace content_settings {

COMPONENT_EXPORT(CONTENT_SETTINGS_FEATURES)
BASE_DECLARE_FEATURE(kAllowIncognitoPermissionInheritance);

}  // namespace content_settings

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_CONTENT_SETTINGS_CORE_COMMON_FEATURES_H_
