/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_COMMON_FEATURES_H_
#define HNS_COMPONENTS_HNS_WALLET_COMMON_FEATURES_H_

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace hns_wallet {
namespace features {

BASE_DECLARE_FEATURE(kNativeHnsWalletFeature);
extern const base::FeatureParam<bool> kShowToolbarTxStatus;
BASE_DECLARE_FEATURE(kHnsWalletFilecoinFeature);
BASE_DECLARE_FEATURE(kHnsWalletSolanaFeature);
BASE_DECLARE_FEATURE(kHnsWalletNftPinningFeature);
BASE_DECLARE_FEATURE(kHnsWalletPanelV2Feature);
extern const base::FeatureParam<bool> kCreateDefaultSolanaAccount;
BASE_DECLARE_FEATURE(kHnsWalletSolanaProviderFeature);
BASE_DECLARE_FEATURE(kHnsWalletDappsSupportFeature);
BASE_DECLARE_FEATURE(kHnsWalletENSL2Feature);
BASE_DECLARE_FEATURE(kHnsWalletSnsFeature);
BASE_DECLARE_FEATURE(kHnsWalletBitcoinFeature);

}  // namespace features
}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_COMMON_FEATURES_H_
