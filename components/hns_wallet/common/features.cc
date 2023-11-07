/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/common/features.h"

#include "base/feature_list.h"
#include "build/build_config.h"

namespace hns_wallet {
namespace features {

BASE_FEATURE(kNativeHnsWalletFeature,
             "NativeHnsWallet",
             base::FEATURE_ENABLED_BY_DEFAULT);
const base::FeatureParam<bool> kShowToolbarTxStatus{
    &kNativeHnsWalletFeature, "show_toolbar_tx_status", true};

BASE_FEATURE(kHnsWalletFilecoinFeature,
             "HnsWalletFilecoin",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kHnsWalletSolanaFeature,
             "HnsWalletSolana",
             base::FEATURE_ENABLED_BY_DEFAULT);
const base::FeatureParam<bool> kCreateDefaultSolanaAccount{
    &kHnsWalletSolanaFeature, "create_default_solana_account", true};

BASE_FEATURE(kHnsWalletSolanaProviderFeature,
             "HnsWalletSolanaProvider",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kHnsWalletDappsSupportFeature,
             "HnsWalletDappsSupport",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kHnsWalletENSL2Feature,
             "HnsWalletENSL2",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kHnsWalletSnsFeature,
             "HnsWalletSns",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kHnsWalletNftPinningFeature,
             "HnsWalletNftPinning",
#if BUILDFLAG(IS_ANDROID)
             base::FEATURE_DISABLED_BY_DEFAULT
#else
             base::FEATURE_ENABLED_BY_DEFAULT
#endif
);

BASE_FEATURE(kHnsWalletPanelV2Feature,
             "HnsWalletPanelV2",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kHnsWalletBitcoinFeature,
             "HnsWalletBitcoin",
             base::FEATURE_DISABLED_BY_DEFAULT);

}  // namespace features
}  // namespace hns_wallet
