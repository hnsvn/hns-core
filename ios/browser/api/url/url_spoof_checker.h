/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_URL_URL_SPOOF_CHECKER_H_
#define HNS_IOS_BROWSER_API_URL_URL_SPOOF_CHECKER_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// The |SkeletonType| and |TopDomainEntry| are mirrored in trie_entry.h. These
/// are used to insert and read nodes from the Trie.
/// The type of skeleton in the trie node.
typedef NSInteger HnsSpoofCheckerSkeletonType NS_TYPED_ENUM
    NS_SWIFT_NAME(URLSpoofChecker.SkeletonType);
OBJC_EXPORT HnsSpoofCheckerSkeletonType const
    HnsSpoofCheckerSkeletonTypeFull;
OBJC_EXPORT HnsSpoofCheckerSkeletonType const
    HnsSpoofCheckerSkeletonTypeSeparatorsRemoved;

typedef NSInteger HnsSpoofCheckerLookalikeURLMatchType NS_TYPED_ENUM
    NS_SWIFT_NAME(URLSpoofChecker.LookalikeURLMatchType);

OBJC_EXPORT HnsSpoofCheckerLookalikeURLMatchType const
    HnsSpoofCheckerLookalikeURLMatchTypeNone;
OBJC_EXPORT HnsSpoofCheckerLookalikeURLMatchType const
    HnsSpoofCheckerLookalikeURLMatchTypeSkeletonMatchSiteEngagement;
OBJC_EXPORT HnsSpoofCheckerLookalikeURLMatchType const
    HnsSpoofCheckerLookalikeURLMatchTypeEditDistance;
OBJC_EXPORT HnsSpoofCheckerLookalikeURLMatchType const
    HnsSpoofCheckerLookalikeURLMatchTypeEditDistanceSiteEngagement;
OBJC_EXPORT HnsSpoofCheckerLookalikeURLMatchType const
    HnsSpoofCheckerLookalikeURLMatchTypeTargetEmbedding;
OBJC_EXPORT HnsSpoofCheckerLookalikeURLMatchType const
    HnsSpoofCheckerLookalikeURLMatchTypeSkeletonMatchTop500;
OBJC_EXPORT HnsSpoofCheckerLookalikeURLMatchType const
    HnsSpoofCheckerLookalikeURLMatchTypeSkeletonMatchTop5k;
OBJC_EXPORT HnsSpoofCheckerLookalikeURLMatchType const
    HnsSpoofCheckerLookalikeURLMatchTypeTargetEmbeddingForSafetyTips;
/// The domain name failed IDN spoof checks but didn't match a safe hostname.
/// As a result, there is no URL to suggest to the user in the form of "Did
/// you mean <url>?".
OBJC_EXPORT HnsSpoofCheckerLookalikeURLMatchType const
    HnsSpoofCheckerLookalikeURLMatchTypeFailedSpoofChecks;
OBJC_EXPORT HnsSpoofCheckerLookalikeURLMatchType const
    HnsSpoofCheckerLookalikeURLMatchTypeCharacterSwapSiteEngagement;
OBJC_EXPORT HnsSpoofCheckerLookalikeURLMatchType const
    HnsSpoofCheckerLookalikeURLMatchTypeCharacterSwapTop500;

OBJC_EXPORT
NS_SWIFT_NAME(URLSpoofChecker.TopDomainEntry)
@interface URLSpoofCheckerTopDomainEntry : NSObject
- (instancetype)init NS_UNAVAILABLE;
/// The domain name.
@property(nonatomic, readonly) NSString* domain;
/// True if the domain is in the top 1000 bucket.
@property(nonatomic, readonly) bool isTopBucket;
/// Type of the skeleton stored in the trie node.
@property(nonatomic, readonly) HnsSpoofCheckerSkeletonType skeletonType;
@end

OBJC_EXPORT
NS_SWIFT_NAME(URLSpoofChecker.Result)
@interface HnsURLSpoofCheckerResult : NSObject
- (instancetype)init NS_UNAVAILABLE;
@property(nonatomic, readonly)
    HnsSpoofCheckerLookalikeURLMatchType urlMatchType;
@property(nonatomic, readonly, nullable) NSURL* suggestedURL;
@end

OBJC_EXPORT
NS_SWIFT_NAME(URLSpoofChecker)
@interface HnsURLSpoofChecker : NSObject
- (instancetype)init NS_UNAVAILABLE;
+ (URLSpoofCheckerTopDomainEntry*)getSimilarTopDomain:(NSString*)hostname;
+ (URLSpoofCheckerTopDomainEntry*)lookupSkeletonInTopDomains:
    (NSString*)hostname;
+ (NSArray<NSString*>*)getSkeletons:(NSString*)url;
+ (HnsURLSpoofCheckerResult*)isLookalikeURL:(NSString*)url;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_URL_URL_SPOOF_CHECKER_H_
