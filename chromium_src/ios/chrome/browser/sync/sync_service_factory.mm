/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/sync/hns_sync_service_impl_delegate.h"
#include "hns/components/sync/service/hns_sync_service_impl.h"
#include "ios/chrome/browser/sync/device_info_sync_service_factory.h"

#define HNS_BUILD_SERVICE_INSTANCE_FOR                      \
  std::make_unique<syncer::HnsSyncServiceImpl>(             \
      std::move(init_params),                                 \
      std::make_unique<syncer::HnsSyncServiceImplDelegate>( \
          DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state)));

#include "src/ios/chrome/browser/sync/sync_service_factory.mm"

#undef HNS_BUILD_SERVICE_INSTANCE_FOR
