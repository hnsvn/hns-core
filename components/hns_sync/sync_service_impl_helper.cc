/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_sync/sync_service_impl_helper.h"

#include <string>
#include <utility>

#include "hns/components/sync/service/hns_sync_service_impl.h"
#include "components/sync_device_info/device_info_sync_service.h"
#include "components/sync_device_info/device_info_tracker.h"
#include "components/sync_device_info/local_device_info_provider.h"

namespace hns_sync {

void ResetSync(syncer::HnsSyncServiceImpl* sync_service_impl,
               syncer::DeviceInfoSyncService* device_info_service,
               base::OnceClosure on_reset_done) {
  if (sync_service_impl->GetTransportState() !=
      syncer::SyncService::TransportState::ACTIVE) {
    sync_service_impl->OnSelfDeviceInfoDeleted(std::move(on_reset_done));
    return;
  }
  syncer::DeviceInfoTracker* tracker =
      device_info_service->GetDeviceInfoTracker();
  DCHECK(tracker);

  const syncer::DeviceInfo* local_device_info =
      device_info_service->GetLocalDeviceInfoProvider()->GetLocalDeviceInfo();

  // Remove DCHECK when will be found the reason of the issue
  // https://github.com/hnsvn/hns-browser/issues/16066 .
  DCHECK(local_device_info);
  if (!local_device_info) {
    std::move(on_reset_done).Run();
    return;
  }

  sync_service_impl->SuspendDeviceObserverForOwnReset();

  tracker->DeleteDeviceInfo(
      local_device_info->guid(),
      base::BindOnce(
          [](syncer::HnsSyncServiceImpl* sync_service_impl,
             base::OnceClosure on_reset_done) {
            sync_service_impl->OnSelfDeviceInfoDeleted(
                std::move(on_reset_done));
            sync_service_impl->ResumeDeviceObserver();
          },
          sync_service_impl, std::move(on_reset_done)));
}

void DeleteDevice(syncer::HnsSyncServiceImpl* sync_service_impl,
                  syncer::DeviceInfoSyncService* device_info_service,
                  const std::string& device_guid) {
  if (sync_service_impl->GetTransportState() !=
      syncer::SyncService::TransportState::ACTIVE) {
    return;
  }
  syncer::DeviceInfoTracker* tracker =
      device_info_service->GetDeviceInfoTracker();
  DCHECK(tracker);

  tracker->DeleteDeviceInfo(device_guid, base::DoNothing());
}

}  // namespace hns_sync
