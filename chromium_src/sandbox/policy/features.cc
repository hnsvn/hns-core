/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "src/sandbox/policy/features.cc"

namespace sandbox::policy::features {

BASE_FEATURE(kModuleFileNamePatch,
             "ModuleFileNamePatch",
             base::FEATURE_DISABLED_BY_DEFAULT);

}  // namespace sandbox::policy::features
