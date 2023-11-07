/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_RENDERER_WORKER_CONTENT_SETTINGS_CLIENT_H_
#define HNS_CHROMIUM_SRC_CHROME_RENDERER_WORKER_CONTENT_SETTINGS_CLIENT_H_

#define HNS_WORKER_CONTENT_SETTINGS_CLIENT_H                       \
  HnsFarblingLevel GetHnsFarblingLevel() override;               \
  blink::WebSecurityOrigin GetEphemeralStorageOriginSync() override; \
  bool HasContentSettingsRules() const override;

#include "src/chrome/renderer/worker_content_settings_client.h"  // IWYU pragma: export

#undef HNS_WORKER_CONTENT_SETTINGS_CLIENT_H

#endif  // HNS_CHROMIUM_SRC_CHROME_RENDERER_WORKER_CONTENT_SETTINGS_CLIENT_H_
