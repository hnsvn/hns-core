/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_SANDBOX_POLICY_FEATURES_H_
#define HNS_CHROMIUM_SRC_SANDBOX_POLICY_FEATURES_H_

#include "src/sandbox/policy/features.h"  // IWYU pragma: export

namespace sandbox::policy::features {

// Enables patching of executable's name from hns.exe to chrome.exe in
// sandboxed processes.
SANDBOX_POLICY_EXPORT BASE_DECLARE_FEATURE(kModuleFileNamePatch);

}  // namespace sandbox::policy::features

#endif  // HNS_CHROMIUM_SRC_SANDBOX_POLICY_FEATURES_H_
