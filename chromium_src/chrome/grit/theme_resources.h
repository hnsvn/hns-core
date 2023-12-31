// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_CHROMIUM_SRC_CHROME_GRIT_THEME_RESOURCES_H_
#define HNS_CHROMIUM_SRC_CHROME_GRIT_THEME_RESOURCES_H_

#include "build/build_config.h"

#if !BUILDFLAG(IS_ANDROID)
#include "hns/grit/hns_theme_resources.h"  // IWYU pragma: export
#endif

#include "../gen/chrome/grit/theme_resources.h"  // IWYU pragma: export

#endif  // HNS_CHROMIUM_SRC_CHROME_GRIT_THEME_RESOURCES_H_
