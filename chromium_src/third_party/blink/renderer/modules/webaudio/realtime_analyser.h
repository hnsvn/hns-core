/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_WEBAUDIO_REALTIME_ANALYSER_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_WEBAUDIO_REALTIME_ANALYSER_H_

#include <utility>

#include "hns/third_party/blink/renderer/platform/hns_audio_farbling_helper.h"

#define analysis_frame_                                  \
  analysis_frame_;                                       \
                                                         \
 public:                                                 \
  void set_audio_farbling_helper(                        \
      absl::optional<HnsAudioFarblingHelper> helper) { \
    audio_farbling_helper_ = std::move(helper);          \
  }                                                      \
                                                         \
 private:                                                \
  absl::optional<HnsAudioFarblingHelper> audio_farbling_helper_

#include "src/third_party/blink/renderer/modules/webaudio/realtime_analyser.h"  // IWYU pragma: export

#undef analysis_frame_

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_WEBAUDIO_REALTIME_ANALYSER_H_
