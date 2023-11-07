/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_PAGE_CHROME_CLIENT_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_PAGE_CHROME_CLIENT_H_

#define GetScreenInfos                          \
  HnsGetScreenInfos(LocalFrame& frame) const; \
  virtual const display::ScreenInfos& GetScreenInfos

#include "src/third_party/blink/renderer/core/page/chrome_client.h"  // IWYU pragma: export

#undef GetScreenInfos

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_PAGE_CHROME_CLIENT_H_
