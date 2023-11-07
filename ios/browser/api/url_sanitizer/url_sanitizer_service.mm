// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/ios/browser/api/url_sanitizer/url_sanitizer_service+private.h"

#include <string>

#include "base/memory/raw_ptr.h"
#include "hns/components/url_sanitizer/browser/url_sanitizer_service.h"
#import "net/base/mac/url_conversions.h"
#include "url/gurl.h"

@interface URLSanitizerService () {
  raw_ptr<hns::URLSanitizerService> urlSanitizer_;  // NOT OWNED
}

@end

@implementation URLSanitizerService

- (instancetype)initWithURLSanitizerService:
    (hns::URLSanitizerService*)urlSanitizer {
  self = [super init];
  if (self) {
    urlSanitizer_ = urlSanitizer;
  }
  return self;
}

- (nullable NSURL*)sanitizeURL:(NSURL*)url {
  DCHECK(urlSanitizer_);
  GURL gurl = net::GURLWithNSURL(url);
  GURL cleanURL = urlSanitizer_->SanitizeURL(gurl);
  return net::NSURLWithGURL(cleanURL);
}

@end
