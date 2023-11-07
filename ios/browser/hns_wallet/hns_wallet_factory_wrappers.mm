/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/hns_wallet/hns_wallet_factory_wrappers.h"

#include "hns/ios/browser/api/hns_wallet/hns_wallet.mojom.objc+private.h"
#include "hns/ios/browser/hns_wallet/asset_ratio_service_factory.h"
#include "hns/ios/browser/hns_wallet/hns_wallet_ipfs_service_factory.h"
#include "hns/ios/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/ios/browser/hns_wallet/json_rpc_service_factory.h"
#include "hns/ios/browser/hns_wallet/keyring_service_factory.h"
#include "hns/ios/browser/hns_wallet/swap_service_factory.h"
#include "hns/ios/browser/hns_wallet/tx_service_factory.h"
#include "hns/ios/browser/keyed_service/keyed_service_factory_wrapper+private.h"
#include "ios/chrome/browser/shared/model/browser_state/chrome_browser_state.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

@implementation HnsWalletAssetRatioServiceFactory
+ (nullable id)serviceForBrowserState:(ChromeBrowserState*)browserState {
  auto service =
      hns_wallet::AssetRatioServiceFactory::GetForBrowserState(browserState);
  if (!service) {
    return nil;
  }
  return [[HnsWalletAssetRatioServiceMojoImpl alloc]
      initWithAssetRatioService:std::move(service)];
}
@end

@implementation HnsWalletJsonRpcServiceFactory
+ (nullable id)serviceForBrowserState:(ChromeBrowserState*)browserState {
  auto service =
      hns_wallet::JsonRpcServiceFactory::GetForBrowserState(browserState);
  if (!service) {
    return nil;
  }
  return [[HnsWalletJsonRpcServiceMojoImpl alloc]
      initWithJsonRpcService:std::move(service)];
}
@end

@implementation HnsWalletTxServiceFactory
+ (nullable id)serviceForBrowserState:(ChromeBrowserState*)browserState {
  auto service =
      hns_wallet::TxServiceFactory::GetForBrowserState(browserState);
  if (!service) {
    return nil;
  }
  return [[HnsWalletTxServiceMojoImpl alloc]
      initWithTxService:std::move(service)];
}
@end

@implementation HnsWalletEthTxManagerProxyFactory
+ (nullable id)serviceForBrowserState:(ChromeBrowserState*)browserState {
  auto proxy =
      hns_wallet::TxServiceFactory::GetEthTxManagerProxyForBrowserState(
          browserState);
  if (!proxy) {
    return nil;
  }
  return [[HnsWalletEthTxManagerProxyMojoImpl alloc]
      initWithEthTxManagerProxy:std::move(proxy)];
}
@end

@implementation HnsWalletSolanaTxManagerProxyFactory
+ (nullable id)serviceForBrowserState:(ChromeBrowserState*)browserState {
  auto proxy =
      hns_wallet::TxServiceFactory::GetSolanaTxManagerProxyForBrowserState(
          browserState);
  if (!proxy) {
    return nil;
  }
  return [[HnsWalletSolanaTxManagerProxyMojoImpl alloc]
      initWithSolanaTxManagerProxy:std::move(proxy)];
}
@end

@implementation HnsWalletKeyringServiceFactory
+ (nullable id)serviceForBrowserState:(ChromeBrowserState*)browserState {
  auto service =
      hns_wallet::KeyringServiceFactory::GetForBrowserState(browserState);
  if (!service) {
    return nil;
  }
  return [[HnsWalletKeyringServiceMojoImpl alloc]
      initWithKeyringService:std::move(service)];
}
@end

@implementation HnsWalletServiceFactory
+ (nullable id)serviceForBrowserState:(ChromeBrowserState*)browserState {
  auto service =
      hns_wallet::HnsWalletServiceFactory::GetForBrowserState(browserState);
  if (!service) {
    return nil;
  }
  return [[HnsWalletHnsWalletServiceMojoImpl alloc]
      initWithHnsWalletService:std::move(service)];
}
@end

@implementation HnsWalletSwapServiceFactory
+ (nullable id)serviceForBrowserState:(ChromeBrowserState*)browserState {
  auto service =
      hns_wallet::SwapServiceFactory::GetForBrowserState(browserState);
  if (!service) {
    return nil;
  }
  return [[HnsWalletSwapServiceMojoImpl alloc]
      initWithSwapService:std::move(service)];
}
@end

@implementation HnsWalletIpfsServiceFactory
+ (nullable id)serviceForBrowserState:(ChromeBrowserState*)browserState {
  auto service =
      hns_wallet::HnsWalletIpfsServiceFactory::GetForBrowserState(
          browserState);
  if (!service) {
    return nil;
  }
  return [[HnsWalletIpfsServiceMojoImpl alloc]
      initWithIpfsService:std::move(service)];
}
@end
