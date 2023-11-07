/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_DOM_DOCUMENT_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_DOM_DOCUMENT_H_

#define ProcessJavaScriptUrl(...)                 \
  ProcessJavaScriptUrl_ChromiumImpl(__VA_ARGS__); \
  void ProcessJavaScriptUrl(__VA_ARGS__)

#include "src/third_party/blink/renderer/core/dom/document.h"  // IWYU pragma: export

#undef ProcessJavaScriptUrl

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_DOM_DOCUMENT_H_
