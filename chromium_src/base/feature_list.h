/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_BASE_FEATURE_LIST_H_
#define HNS_CHROMIUM_SRC_BASE_FEATURE_LIST_H_

#define IsFeatureOverridden                                                \
  IsFeatureOverridden_ChromiumImpl(const std::string& feature_name) const; \
  static FeatureState GetCompileTimeFeatureState(const Feature& feature);  \
  bool IsFeatureOverridden

#include "src/base/feature_list.h"  // IWYU pragma: export

#undef IsFeatureOverridden

#endif  // HNS_CHROMIUM_SRC_BASE_FEATURE_LIST_H_