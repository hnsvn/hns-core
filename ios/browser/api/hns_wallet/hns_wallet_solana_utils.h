/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_HNS_WALLET_HNS_WALLET_SOLANA_UTILS_H_
#define HNS_IOS_BROWSER_API_HNS_WALLET_HNS_WALLET_SOLANA_UTILS_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSInteger, NSDataBase58DecodingOptions) {
  NSDataBase58DecodingOptionsNone = 0,
  NSDataBase58DecodingOptionsStrict = 1 << 0
};

@interface NSData (Base58)

/// Initializes a Data object with the given Base58 encoded string.
+ (nullable instancetype)dataWithBase58EncodedString:(NSString*)base58String
    NS_SWIFT_NAME(init(base58Encoded:));

/// Initializes a Data object with the given Base58 encoded string with options
+ (nullable instancetype)
    dataWithBase58EncodedString:(NSString*)base58String
                        options:(NSDataBase58DecodingOptions)options
    NS_SWIFT_NAME(init(base58Encoded:options:));

/// Returns a Base 58 encoded string representation of the current Data object
- (NSString*)base58EncodedString;

@end

@interface NSString (Base58)

/// Whether or not the given string is a base 58 encoded solana public key
@property(readonly) bool isBase58EncodedSolanaPubkey;

@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_HNS_WALLET_HNS_WALLET_SOLANA_UTILS_H_
