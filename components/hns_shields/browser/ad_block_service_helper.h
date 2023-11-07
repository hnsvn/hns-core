/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_SERVICE_HELPER_H_
#define HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_SERVICE_HELPER_H_

#include <stdint.h>
#include <string>
#include <vector>

#include "base/values.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_shields {

void MergeCspDirectiveInto(absl::optional<std::string> from,
                           absl::optional<std::string>* into);

void MergeResourcesInto(base::Value::Dict from,
                        base::Value::Dict& into,
                        bool force_hide);

}  // namespace hns_shields

#endif  // HNS_COMPONENTS_HNS_SHIELDS_BROWSER_AD_BLOCK_SERVICE_HELPER_H_
