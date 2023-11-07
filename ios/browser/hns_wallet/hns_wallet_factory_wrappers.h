/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_HNS_WALLET_HNS_WALLET_FACTORY_WRAPPERS_H_
#define HNS_IOS_BROWSER_HNS_WALLET_HNS_WALLET_FACTORY_WRAPPERS_H_

#import <Foundation/Foundation.h>
#include "keyed_service_factory_wrapper.h"  // NOLINT

@protocol HnsWalletAssetRatioService
, HnsWalletHnsWalletService, HnsWalletJsonRpcService,
    HnsWalletEthTxManagerProxy, HnsWalletSolanaTxManagerProxy,
    HnsWalletTxService, HnsWalletKeyringService, HnsWalletSwapService,
    HnsWalletIpfsService;

OBJC_EXPORT
NS_SWIFT_NAME(HnsWallet.AssetRatioServiceFactory)
@interface HnsWalletAssetRatioServiceFactory
    : KeyedServiceFactoryWrapper <id <HnsWalletAssetRatioService>>
@end

OBJC_EXPORT
NS_SWIFT_NAME(HnsWallet.JsonRpcServiceFactory)
@interface HnsWalletJsonRpcServiceFactory
    : KeyedServiceFactoryWrapper <id <HnsWalletJsonRpcService>>
@end

OBJC_EXPORT
NS_SWIFT_NAME(HnsWallet.TxServiceFactory)
@interface HnsWalletTxServiceFactory
    : KeyedServiceFactoryWrapper <id <HnsWalletTxService>>
@end

OBJC_EXPORT
NS_SWIFT_NAME(HnsWallet.EthTxManagerProxyFactory)
@interface HnsWalletEthTxManagerProxyFactory
    : KeyedServiceFactoryWrapper <id <HnsWalletEthTxManagerProxy>>
@end

OBJC_EXPORT
NS_SWIFT_NAME(HnsWallet.SolanaTxManagerProxyFactory)
@interface HnsWalletSolanaTxManagerProxyFactory
    : KeyedServiceFactoryWrapper <id <HnsWalletSolanaTxManagerProxy>>
@end

OBJC_EXPORT
NS_SWIFT_NAME(HnsWallet.KeyringServiceFactory)
@interface HnsWalletKeyringServiceFactory
    : KeyedServiceFactoryWrapper <id <HnsWalletKeyringService>>
@end

OBJC_EXPORT
NS_SWIFT_NAME(HnsWallet.ServiceFactory)
@interface HnsWalletServiceFactory
    : KeyedServiceFactoryWrapper <id <HnsWalletHnsWalletService>>
@end

OBJC_EXPORT
NS_SWIFT_NAME(HnsWallet.SwapServiceFactory)
@interface HnsWalletSwapServiceFactory
    : KeyedServiceFactoryWrapper <id <HnsWalletSwapService>>
@end

OBJC_EXPORT
NS_SWIFT_NAME(HnsWallet.IpfsServiceFactory)
@interface HnsWalletIpfsServiceFactory
    : KeyedServiceFactoryWrapper <id <HnsWalletIpfsService>>
@end

#endif  // HNS_IOS_BROWSER_HNS_WALLET_HNS_WALLET_FACTORY_WRAPPERS_H_
