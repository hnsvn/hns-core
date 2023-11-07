/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define HNS_REALTIMEANALYSER_CONVERTFLOATTODB                               \
  if (audio_farbling_helper_) {                                               \
    audio_farbling_helper_->FarbleConvertFloatToDb(source, destination, len); \
    return;                                                                   \
  }

#define HNS_REALTIMEANALYSER_CONVERTTOBYTEDATA                     \
  if (audio_farbling_helper_) {                                      \
    audio_farbling_helper_->FarbleConvertToByteData(                 \
        source, destination, len, min_decibels, range_scale_factor); \
    return;                                                          \
  }

#define HNS_REALTIMEANALYSER_GETFLOATTIMEDOMAINDATA          \
  if (audio_farbling_helper_) {                                \
    audio_farbling_helper_->FarbleFloatTimeDomainData(         \
        input_buffer, destination, len, write_index, fft_size, \
        kInputBufferSize);                                     \
    return;                                                    \
  }

#define HNS_REALTIMEANALYSER_GETBYTETIMEDOMAINDATA           \
  if (audio_farbling_helper_) {                                \
    audio_farbling_helper_->FarbleByteTimeDomainData(          \
        input_buffer, destination, len, write_index, fft_size, \
        kInputBufferSize);                                     \
    return;                                                    \
  }

#include "src/third_party/blink/renderer/modules/webaudio/realtime_analyser.cc"

#undef HNS_REALTIMEANALYSER_CONVERTFLOATTODB
#undef HNS_REALTIMEANALYSER_CONVERTTOBYTEDATA
#undef HNS_REALTIMEANALYSER_GETFLOATTIMEDOMAINDATA
#undef HNS_REALTIMEANALYSER_GETBYTETIMEDOMAINDATA
