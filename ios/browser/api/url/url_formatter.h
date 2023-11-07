/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_URL_URL_FORMATTER_H_
#define HNS_IOS_BROWSER_API_URL_URL_FORMATTER_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NSInteger HnsURLSchemeDisplay NS_TYPED_ENUM
    NS_SWIFT_NAME(URLFormatter.SchemeDisplay);

OBJC_EXPORT HnsURLSchemeDisplay const HnsURLSchemeDisplayShow;
OBJC_EXPORT HnsURLSchemeDisplay const HnsURLSchemeDisplayOmitHttpAndHttps;
/// Omit cryptographic (i.e. https and wss).
OBJC_EXPORT HnsURLSchemeDisplay const HnsURLSchemeDisplayOmitCryptographic;

NS_SWIFT_NAME(URLFormatter.FormatType)
typedef NS_OPTIONS(NSUInteger, HnsURLFormatterFormatType) {
  HnsURLFormatterFormatTypeOmitNothing = 0,
  HnsURLFormatterFormatTypeOmitUsernamePassword = 1 << 0,
  HnsURLFormatterFormatTypeOmitHTTP = 1 << 1,
  HnsURLFormatterFormatTypeOmitTrailingSlashOnBareHostname = 1 << 2,
  HnsURLFormatterFormatTypeOmitHTTPS = 1 << 3,
  HnsURLFormatterFormatTypeOmitTrivialSubdomains = 1 << 5,
  HnsURLFormatterFormatTypeTrimAfterHost = 1 << 6,
  HnsURLFormatterFormatTypeOmitFileScheme = 1 << 7,
  HnsURLFormatterFormatTypeOmitMailToScheme = 1 << 8,
  HnsURLFormatterFormatTypeOmitMobilePrefix = 1 << 9,

  /// Omits Username & Password, HTTP (not HTTPS), and Trailing Slash
  HnsURLFormatterFormatTypeOmitDefaults =
      HnsURLFormatterFormatTypeOmitUsernamePassword |
      HnsURLFormatterFormatTypeOmitHTTP |
      HnsURLFormatterFormatTypeOmitTrailingSlashOnBareHostname
};

NS_SWIFT_NAME(URLFormatter.UnescapeRule)
typedef NS_OPTIONS(NSUInteger, HnsURLFormatterUnescapeRule) {
  HnsURLFormatterUnescapeRuleNone = 0,
  HnsURLFormatterUnescapeRuleNormal = 1 << 0,
  HnsURLFormatterUnescapeRuleSpaces = 1 << 1,
  HnsURLFormatterUnescapeRulePathSeparators = 1 << 2,
  HnsURLFormatterUnescapeRuleSpecialCharsExceptPathSeparators = 1 << 3,
  HnsURLFormatterUnescapeRuleReplacePlusWithSpace = 1 << 4
};

OBJC_EXPORT
NS_SWIFT_NAME(URLFormatter)
@interface HnsURLFormatter : NSObject
- (instancetype)init NS_UNAVAILABLE;

/// Format a URL "origin/host" for Security Display
/// origin - The origin of the URL to format
/// schemeDisplay - Determines whether or not to omit the scheme
+ (NSString*)formatURLOriginForSecurityDisplay:(NSString*)origin
                                 schemeDisplay:
                                     (HnsURLSchemeDisplay)schemeDisplay;

/// Format a URL "origin/host" omitting the scheme, path, and trivial
/// sub-domains. origin - The origin to be formatted
+ (NSString*)formatURLOriginForDisplayOmitSchemePathAndTrivialSubdomains:
    (NSString*)origin;

/// Format a URL
/// url - The URL string to be formatted
/// formatTypes - Formatter options when formatting the URL. Typically used to
/// omit certain parts of a URL unescapeOptions - Options passed to the
/// formatter for UN-Escaping parts of a URL
+ (NSString*)formatURL:(NSString*)url
           formatTypes:(HnsURLFormatterFormatType)formatTypes
       unescapeOptions:(HnsURLFormatterUnescapeRule)unescapeOptions;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_URL_URL_FORMATTER_H_
