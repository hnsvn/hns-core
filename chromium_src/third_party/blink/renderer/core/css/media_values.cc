/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "third_party/blink/renderer/core/css/media_values.h"

#include "hns/third_party/blink/renderer/core/farbling/hns_session_cache.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/screen.h"

#define CalculateDeviceWidth                                                   \
  CalculateDeviceWidth(LocalFrame* frame) {                                    \
    ExecutionContext* context = frame->DomWindow()->GetExecutionContext();     \
    auto* top_frame = DynamicTo<LocalFrame>(frame->Top());                     \
    return top_frame && hns::BlockScreenFingerprinting(context)              \
               ? hns::FarbleInteger(context,                                 \
                                      hns::FarbleKey::kWindowInnerWidth,     \
                                      CalculateViewportWidth(top_frame), 0, 8) \
               : CalculateDeviceWidth_ChromiumImpl(frame);                     \
  }                                                                            \
  int MediaValues::CalculateDeviceWidth_ChromiumImpl

#define CalculateDeviceHeight                                              \
  CalculateDeviceHeight(LocalFrame* frame) {                               \
    ExecutionContext* context = frame->DomWindow()->GetExecutionContext(); \
    auto* top_frame = DynamicTo<LocalFrame>(frame->Top());                 \
    return top_frame && hns::BlockScreenFingerprinting(context)          \
               ? hns::FarbleInteger(                                     \
                     context, hns::FarbleKey::kWindowInnerHeight,        \
                     CalculateViewportHeight(top_frame), 0, 8)             \
               : CalculateDeviceHeight_ChromiumImpl(frame);                \
  }                                                                        \
  int MediaValues::CalculateDeviceHeight_ChromiumImpl

#include "src/third_party/blink/renderer/core/css/media_values.cc"

#undef CalculateDeviceWidth
#undef CalculateDeviceHeight
