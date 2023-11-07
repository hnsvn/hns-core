/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "components/content_settings/core/common/content_settings_types.h"

// clang-format off
#define TOP_LEVEL_STORAGE_ACCESS                     \
                           TOP_LEVEL_STORAGE_ACCESS, \
      ContentSettingsType::HNS_FINGERPRINTING_V2,  \
      ContentSettingsType::HNS_HTTPS_UPGRADE
// clang-format on

#include "src/chrome/browser/ui/webui/settings/safety_hub_handler_unittest.cc"
#undef TOP_LEVEL_STORAGE_ACCESS
