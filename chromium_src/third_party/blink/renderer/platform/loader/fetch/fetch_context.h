/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_PLATFORM_LOADER_FETCH_FETCH_CONTEXT_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_PLATFORM_LOADER_FETCH_FETCH_CONTEXT_H_

#define AddAdditionalRequestHeaders                             \
  NotUsed() {}                                                  \
  virtual String GetCacheIdentifierIfCrossSiteSubframe() const; \
  virtual void AddAdditionalRequestHeaders

#include "src/third_party/blink/renderer/platform/loader/fetch/fetch_context.h"  // IWYU pragma: export

#undef AddAdditionalRequestHeaders

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_PLATFORM_LOADER_FETCH_FETCH_CONTEXT_H_
