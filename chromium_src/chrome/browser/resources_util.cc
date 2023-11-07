/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "build/build_config.h"

#if !BUILDFLAG(IS_ANDROID)

#include "hns/grit/hns_theme_resources_map.h"

#define HNS_RESOURCES_UTIL                              \
  for (size_t i = 0; i < kHnsThemeResourcesSize; ++i) { \
    storage.emplace_back(kHnsThemeResources[i].path,    \
                         kHnsThemeResources[i].id);     \
  }

#else
#define HNS_RESOURCES_UTIL
#endif  // !BUILDFLAG(IS_ANDROID)

#include "src/chrome/browser/resources_util.cc"
#undef HNS_RESOURCES_UTIL
