/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_HNS_WALLET_HNS_WALLET_PROVIDER_DELEGATE_IOS_H_
#define HNS_IOS_BROWSER_API_HNS_WALLET_HNS_WALLET_PROVIDER_DELEGATE_IOS_H_

#import <Foundation/Foundation.h>
#import "hns_wallet.mojom.objc.h"

@class URLOriginIOS;

NS_ASSUME_NONNULL_BEGIN

typedef void (^RequestPermissionsCallback)(
    HnsWalletRequestPermissionsError error,
    NSArray<NSString*>* _Nullable allowedAccounts);

OBJC_EXPORT
@protocol HnsWalletProviderDelegate
- (bool)isTabVisible;
- (void)showPanel;
- (URLOriginIOS*)getOrigin;
- (void)walletInteractionDetected;
- (void)showWalletOnboarding;
- (void)showAccountCreation:(HnsWalletCoinType)type;
- (void)requestPermissions:(HnsWalletCoinType)type
                  accounts:(NSArray<NSString*>*)accounts
                completion:(RequestPermissionsCallback)completion;
- (bool)isAccountAllowed:(HnsWalletCoinType)type account:(NSString*)account;
- (nullable NSArray<NSString*>*)getAllowedAccounts:(HnsWalletCoinType)type
                                          accounts:
                                              (NSArray<NSString*>*)accounts;
- (bool)isPermissionDenied:(HnsWalletCoinType)type;
- (void)addSolanaConnectedAccount:(NSString*)account;
- (void)removeSolanaConnectedAccount:(NSString*)account;
- (bool)isSolanaAccountConnected:(NSString*)account;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_HNS_WALLET_HNS_WALLET_PROVIDER_DELEGATE_IOS_H_
