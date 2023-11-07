/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_UI_VIEWS_INPUT_EVENT_ACTIVATION_PROTECTOR_H_
#define HNS_CHROMIUM_SRC_UI_VIEWS_INPUT_EVENT_ACTIVATION_PROTECTOR_H_

#include "ui/views/windows_stationarity_monitor.h"

#define OnWindowStationaryStateChanged(...)                  \
  OnWindowStationaryStateChanged_ChromiumImpl(__VA_ARGS__);  \
  void IgnoreNextWindowStationaryStateChanged();             \
                                                             \
 private:                                                    \
  bool ignore_next_window_stationary_state_changed_ = false; \
                                                             \
 public:                                                     \
  void OnWindowStationaryStateChanged(__VA_ARGS__)

#include "src/ui/views/input_event_activation_protector.h"  // IWYU pragma: export

#undef OnWindowStationaryStateChanged

#endif  // HNS_CHROMIUM_SRC_UI_VIEWS_INPUT_EVENT_ACTIVATION_PROTECTOR_H_
