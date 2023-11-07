/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_HNS_SHIELDS_ADBLOCK_SERVICE_PRIVATE_H_
#define HNS_IOS_BROWSER_API_HNS_SHIELDS_ADBLOCK_SERVICE_PRIVATE_H_

#include "hns/ios/browser/api/hns_shields/adblock_service.h"

namespace component_updater {
class ComponentUpdateService;
}  // namespace component_updater

@interface AdblockService (Private)
- (instancetype)initWithComponentUpdater:
    (component_updater::ComponentUpdateService*)componentUpdaterService;
- (void)registerDefaultShieldsComponent;
@end

#endif  // HNS_IOS_BROWSER_API_HNS_SHIELDS_ADBLOCK_SERVICE_PRIVATE_H_
