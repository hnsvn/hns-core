/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/api/opentabs/hns_tabgenerator_api.h"
#import "hns/ios/browser/api/web/web_state/web_state+private.h"

#include <memory>

#include "base/strings/sys_string_conversions.h"

#include "ios/chrome/browser/shared/model/browser/browser.h"
#include "ios/web/public/thread/web_thread.h"
#include "ios/web/web_state/web_state_impl.h"

#include "net/base/mac/url_conversions.h"
#include "url/gurl.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

#pragma mark - HnsSyncTab

@interface HnsSyncTab ()
@property(nonatomic, strong) WebState* web_state;
@end

@implementation HnsSyncTab

- (instancetype)initWithBrowser:(Browser*)browser
                 isOffTheRecord:(bool)isOffTheRecord {
  if ((self = [super init])) {
    _web_state = [[WebState alloc] initWithBrowser:browser
                                    isOffTheRecord:isOffTheRecord];
  }
  return self;
}

- (WebState*)webState {
  return _web_state;
}

- (void)setTitle:(NSString*)title {
  [self.web_state setTitle:title];
}

- (void)setURL:(NSURL*)url {
  [self.web_state setURL:url];
}
@end

#pragma mark - HnsTabGeneratorAPI

@interface HnsTabGeneratorAPI () {
  Browser* browser_;
}
@end

@implementation HnsTabGeneratorAPI

- (instancetype)initWithBrowser:(Browser*)browser {
  if ((self = [super init])) {
    DCHECK_CURRENTLY_ON(web::WebThread::UI);
    DCHECK(browser);
    browser_ = browser;
  }
  return self;
}

- (HnsSyncTab*)createHnsSyncTab:(bool)isOffTheRecord {
  return [[HnsSyncTab alloc] initWithBrowser:browser_
                                isOffTheRecord:isOffTheRecord];
}

- (void)dealloc {
  browser_ = nil;
}

@end
