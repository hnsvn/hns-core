/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_TARGETING_USER_MODEL_BUILDER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_TARGETING_USER_MODEL_BUILDER_H_

#include "base/functional/callback.h"

namespace hns_ads {

struct UserModelInfo;

using BuildUserModelCallback =
    base::OnceCallback<void(const UserModelInfo& user_model)>;

void BuildUserModel(BuildUserModelCallback callback);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_TARGETING_USER_MODEL_BUILDER_H_
