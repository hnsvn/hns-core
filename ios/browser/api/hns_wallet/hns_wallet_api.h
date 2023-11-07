/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_HNS_WALLET_HNS_WALLET_API_H_
#define HNS_IOS_BROWSER_API_HNS_WALLET_HNS_WALLET_API_H_

#import <Foundation/Foundation.h>

@protocol HnsWalletBlockchainRegistry;
@protocol HnsWalletEthereumProvider;
@protocol HnsWalletProviderDelegate;
@protocol HnsWalletSolanaProvider;

typedef NS_ENUM(NSInteger, HnsWalletCoinType);

NS_ASSUME_NONNULL_BEGIN

typedef NSString* HnsWalletProviderScriptKey NS_STRING_ENUM;
OBJC_EXPORT HnsWalletProviderScriptKey const
    HnsWalletProviderScriptKeyEthereum;
OBJC_EXPORT HnsWalletProviderScriptKey const
    HnsWalletProviderScriptKeySolana;
OBJC_EXPORT HnsWalletProviderScriptKey const
    HnsWalletProviderScriptKeySolanaWeb3;
OBJC_EXPORT HnsWalletProviderScriptKey const
    HnsWalletProviderScriptKeyWalletStandard;

OBJC_EXPORT
@interface HnsWalletAPI : NSObject

@property(class, readonly) id<HnsWalletBlockchainRegistry> blockchainRegistry;

- (nullable id<HnsWalletEthereumProvider>)
    ethereumProviderWithDelegate:(id<HnsWalletProviderDelegate>)delegate
               isPrivateBrowsing:(bool)isPrivateBrowsing;

- (nullable id<HnsWalletSolanaProvider>)
    solanaProviderWithDelegate:(id<HnsWalletProviderDelegate>)delegate
             isPrivateBrowsing:(bool)isPrivateBrowsing;

- (NSDictionary<HnsWalletProviderScriptKey, NSString*>*)
    providerScriptsForCoinType:(HnsWalletCoinType)coinType;

- (instancetype)init NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_HNS_WALLET_HNS_WALLET_API_H_
