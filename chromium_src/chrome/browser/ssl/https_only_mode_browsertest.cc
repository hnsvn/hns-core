/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "net/base/features.h"

// Prevent re-defining InitWithFeatures definition.
#include "base/test/scoped_feature_list.h"

#define InitWithFeatures(...) \
  InitWithFeaturesAndDisable(net::features::kHnsHttpsByDefault, __VA_ARGS__)

#define InitAndEnableFeature(FEATURE_TO_ENABLE) \
  InitWithFeatures({FEATURE_TO_ENABLE}, {net::features::kHnsHttpsByDefault})

#include "src/chrome/browser/ssl/https_only_mode_browsertest.cc"

#undef InitWithFeatures
#undef InitAndEnableFeature
