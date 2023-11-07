// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_COMMANDS_COMMON_FEATURES_H_
#define HNS_COMPONENTS_COMMANDS_COMMON_FEATURES_H_

#include "base/component_export.h"
#include "base/feature_list.h"

namespace commands::features {

COMPONENT_EXPORT(COMMANDS_COMMON) BASE_DECLARE_FEATURE(kHnsCommands);

}  // namespace commands::features

#endif  // HNS_COMPONENTS_COMMANDS_COMMON_FEATURES_H_
