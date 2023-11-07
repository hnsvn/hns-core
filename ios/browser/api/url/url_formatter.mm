/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/api/url/url_formatter.h"

#include "base/functional/bind.h"
#include "base/functional/callback.h"
#include "base/notreached.h"
#include "base/strings/sys_string_conversions.h"
#include "build/build_config.h"
#include "components/url_formatter/elide_url.h"
#include "components/url_formatter/url_formatter.h"
#include "net/base/mac/url_conversions.h"
#include "url/gurl.h"
#include "url/origin.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

// MARK: - SchemeDisplay
HnsURLSchemeDisplay const HnsURLSchemeDisplayShow =
    static_cast<NSInteger>(url_formatter::SchemeDisplay::SHOW);
HnsURLSchemeDisplay const HnsURLSchemeDisplayOmitHttpAndHttps =
    static_cast<NSInteger>(url_formatter::SchemeDisplay::OMIT_HTTP_AND_HTTPS);
/// Omit cryptographic (i.e. https and wss).
HnsURLSchemeDisplay const HnsURLSchemeDisplayOmitCryptographic =
    static_cast<NSInteger>(url_formatter::SchemeDisplay::OMIT_CRYPTOGRAPHIC);

// MARK: - Implementation

@implementation HnsURLFormatter
+ (NSString*)formatURLOriginForSecurityDisplay:(NSString*)origin
                                 schemeDisplay:
                                     (HnsURLSchemeDisplay)schemeDisplay {
  std::u16string result = url_formatter::FormatUrlForSecurityDisplay(
      GURL(base::SysNSStringToUTF8(origin)),
      static_cast<url_formatter::SchemeDisplay>(schemeDisplay));
  return base::SysUTF16ToNSString(result) ?: @"";
}

+ (NSString*)formatURLOriginForDisplayOmitSchemePathAndTrivialSubdomains:
    (NSString*)origin {
  std::u16string result =
      url_formatter::FormatUrlForDisplayOmitSchemePathAndTrivialSubdomains(
          GURL(base::SysNSStringToUTF8(origin)));
  return base::SysUTF16ToNSString(result) ?: @"";
}

+ (NSString*)formatURL:(NSString*)url
           formatTypes:(HnsURLFormatterFormatType)formatTypes
       unescapeOptions:(HnsURLFormatterUnescapeRule)unescapeOptions {
  std::u16string result = url_formatter::FormatUrl(
      GURL(base::SysNSStringToUTF8(url)),
      static_cast<url_formatter::FormatUrlType>(formatTypes),
      static_cast<base::UnescapeRule::Type>(unescapeOptions), nullptr, nullptr,
      nullptr);
  return base::SysUTF16ToNSString(result) ?: @"";
}
@end
