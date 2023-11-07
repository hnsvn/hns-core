/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_BLUETOOTH_SCANNING_PROMPT_CONTROLLER_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_BLUETOOTH_SCANNING_PROMPT_CONTROLLER_H_

#include "components/permissions/chooser_controller.h"

#define GetThrobberLabelAndTooltip(...)                   \
  GetThrobberLabelAndTooltip(__VA_ARGS__) const override; \
  absl::optional<ChooserControllerType> GetType()

#include "src/components/permissions/bluetooth_scanning_prompt_controller.h"  // IWYU pragma: export

#undef GetThrobberLabelAndTooltip

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_BLUETOOTH_SCANNING_PROMPT_CONTROLLER_H_
