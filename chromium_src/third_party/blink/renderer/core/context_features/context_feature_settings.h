/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_CONTEXT_FEATURES_CONTEXT_FEATURE_SETTINGS_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_CONTEXT_FEATURES_CONTEXT_FEATURE_SETTINGS_H_

// ContextEnabled=ReportingObservers feature methods need to be added since we
// patched ReportingObservers as ContextEnabled into
// third_party/blink/renderer/core/frame/reporting_observer.idl
#define HNS_CONTEXT_FEATURE_SETTINGS_H_      \
 public:                                       \
  void enableReportingObservers(bool enable) { \
    enable_reporting_observers_ = enable;      \
  }                                            \
  bool isReportingObserversEnabled() const {   \
    return enable_reporting_observers_;        \
  }                                            \
                                               \
 private:                                      \
  bool enable_reporting_observers_ = false;    \
                                               \
 public:
//  #define HNS_CONTEXT_FEATURE_SETTINGS_H_

#include "src/third_party/blink/renderer/core/context_features/context_feature_settings.h"  // IWYU pragma: export
#undef HNS_CONTEXT_FEATURE_SETTINGS_H_

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_CONTEXT_FEATURES_CONTEXT_FEATURE_SETTINGS_H_
