/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ethereum_remote_client/features.h"

#include "base/feature_list.h"

namespace ethereum_remote_client {
namespace features {

BASE_FEATURE(kCryptoWalletsForNewInstallsFeature,
             "CryptoWalletsForNewInstallsFeature",
             base::FEATURE_DISABLED_BY_DEFAULT);

}  // namespace features
}  // namespace ethereum_remote_client
