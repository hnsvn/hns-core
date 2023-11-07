/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/about_flags.cc"
#include "hns/components/commander/common/buildflags/buildflags.h"
#include "chrome/common/channel_info.h"
#include "components/autofill/core/browser/autofill_experiments.h"

#if BUILDFLAG(ENABLE_COMMANDER)
#include "hns/components/commander/common/features.h"

// Note: We replace the kQuickCommands feature with the kHnsCommander feature
// so we can use it from //components without DEPS violations.
#define kQuickCommands kHnsCommander
#endif

#define GetChannel                                                        \
  GetChannel();                                                           \
  if (flags_ui::HnsShouldSkipConditionalFeatureEntry(storage, entry)) { \
    return true;                                                          \
  }                                                                       \
  chrome::GetChannel
#include "src/chrome/browser/about_flags.cc"
#undef GetChannel
#undef kQuickCommands
