/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CONTENT_PUBLIC_COMMON_CONTENT_CLIENT_H_
#define HNS_CHROMIUM_SRC_CONTENT_PUBLIC_COMMON_CONTENT_CLIENT_H_

#define AddPlugins                                                \
  AddPlugins(std::vector<content::ContentPluginInfo>* plugins) {} \
  virtual void AddPlugins_ChromiumImpl

#include "src/content/public/common/content_client.h"  // IWYU pragma: export

#undef AddPlugins

#endif  // HNS_CHROMIUM_SRC_CONTENT_PUBLIC_COMMON_CONTENT_CLIENT_H_
