/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/ios/app/hns_core_switches.h"

#include "base/base_switches.h"
#include "base/strings/sys_string_conversions.h"
#include "hns/components/p3a/switches.h"
#include "components/component_updater/component_updater_switches.h"
#include "components/sync/base/command_line_switches.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

const HnsCoreSwitchKey HnsCoreSwitchKeyComponentUpdater =
    base::SysUTF8ToNSString(switches::kComponentUpdater);
const HnsCoreSwitchKey HnsCoreSwitchKeyVModule =
    base::SysUTF8ToNSString(switches::kVModule);
const HnsCoreSwitchKey HnsCoreSwitchKeySyncURL =
    base::SysUTF8ToNSString(syncer::kSyncServiceURL);
// There is no exposed switch for rewards
const HnsCoreSwitchKey HnsCoreSwitchKeyRewardsFlags = @"rewards";
const HnsCoreSwitchKey HnsCoreSwitchKeyP3AUploadIntervalSeconds =
    base::SysUTF8ToNSString(p3a::switches::kP3AUploadIntervalSeconds);
const HnsCoreSwitchKey HnsCoreSwitchKeyP3ADoNotRandomizeUploadInterval =
    base::SysUTF8ToNSString(p3a::switches::kP3ADoNotRandomizeUploadInterval);
const HnsCoreSwitchKey HnsCoreSwitchKeyP3ATypicalRotationIntervalSeconds =
    base::SysUTF8ToNSString(p3a::switches::kP3ATypicalRotationIntervalSeconds);
const HnsCoreSwitchKey HnsCoreSwitchKeyP3AExpressRotationIntervalSeconds =
    base::SysUTF8ToNSString(p3a::switches::kP3AExpressRotationIntervalSeconds);
const HnsCoreSwitchKey HnsCoreSwitchKeyP3ASlowRotationIntervalSeconds =
    base::SysUTF8ToNSString(p3a::switches::kP3ASlowRotationIntervalSeconds);
const HnsCoreSwitchKey HnsCoreSwitchKeyP3AFakeStarEpoch =
    base::SysUTF8ToNSString(p3a::switches::kP3AFakeStarEpoch);
const HnsCoreSwitchKey HnsCoreSwitchKeyP3AJsonUploadServerURL =
    base::SysUTF8ToNSString(p3a::switches::kP3AJsonUploadUrl);
const HnsCoreSwitchKey HnsCoreSwitchKeyP3ACreativeUploadServerURL =
    base::SysUTF8ToNSString(p3a::switches::kP3ACreativeUploadUrl);
const HnsCoreSwitchKey HnsCoreSwitchKeyP2AJsonUploadServerURL =
    base::SysUTF8ToNSString(p3a::switches::kP2AJsonUploadUrl);
const HnsCoreSwitchKey HnsCoreSwitchKeyP3AConstellationUploadServerURL =
    base::SysUTF8ToNSString(p3a::switches::kP3AConstellationUploadUrl);
const HnsCoreSwitchKey HnsCoreSwitchKeyP3ADisableStarAttestation =
    base::SysUTF8ToNSString(p3a::switches::kP3ADisableStarAttestation);
const HnsCoreSwitchKey HnsCoreSwitchKeyP3AStarRandomnessHost =
    base::SysUTF8ToNSString(p3a::switches::kP3AStarRandomnessHost);
const HnsCoreSwitchKey HnsCoreSwitchKeyP3AIgnoreServerErrors =
    base::SysUTF8ToNSString(p3a::switches::kP3AIgnoreServerErrors);

@implementation HnsCoreSwitch
- (instancetype)initWithKey:(HnsCoreSwitchKey)key {
  return [self initWithKey:key value:nil];
}
- (instancetype)initWithKey:(HnsCoreSwitchKey)key
                      value:(nullable NSString*)value {
  if ((self = [super init])) {
    _key = [key copy];
    _value = [value copy];
  }
  return self;
}
@end
