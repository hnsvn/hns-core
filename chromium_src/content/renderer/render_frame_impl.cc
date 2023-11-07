/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "build/build_config.h"

#if BUILDFLAG(IS_ANDROID)
#define HNS_RENDER_FRAME_IMPL_SHOULD_USE_USER_AGENT_OVERRIDE
#else
// clang-format off
#define HNS_RENDER_FRAME_IMPL_SHOULD_USE_USER_AGENT_OVERRIDE \
    return true;                                               \
  if (false)                                                   \
// clang-format on
#endif

#include "src/content/renderer/render_frame_impl.cc"
#undef HNS_RENDER_FRAME_IMPL_SHOULD_USE_USER_AGENT_OVERRIDE

