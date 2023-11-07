/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/browser_state/hns_browser_state_keyed_service_factories.h"

#include "hns/ios/browser/hns_wallet/asset_ratio_service_factory.h"
#include "hns/ios/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/ios/browser/hns_wallet/json_rpc_service_factory.h"
#include "hns/ios/browser/hns_wallet/keyring_service_factory.h"
#include "hns/ios/browser/hns_wallet/swap_service_factory.h"
#include "hns/ios/browser/hns_wallet/tx_service_factory.h"
#include "hns/ios/browser/skus/skus_service_factory.h"
#include "hns/ios/browser/url_sanitizer/url_sanitizer_service_factory+private.h"

namespace hns {

void EnsureBrowserStateKeyedServiceFactoriesBuilt() {
  hns_wallet::AssetRatioServiceFactory::GetInstance();
  hns_wallet::HnsWalletServiceFactory::GetInstance();
  hns_wallet::JsonRpcServiceFactory::GetInstance();
  hns_wallet::TxServiceFactory::GetInstance();
  hns_wallet::KeyringServiceFactory::GetInstance();
  hns_wallet::SwapServiceFactory::GetInstance();
  skus::SkusServiceFactory::GetInstance();
  hns::URLSanitizerServiceFactory::GetInstance();
}

}  // namespace hns
