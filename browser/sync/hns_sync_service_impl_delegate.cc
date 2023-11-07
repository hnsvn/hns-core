/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/sync/hns_sync_service_impl_delegate.h"

#include <algorithm>
#include <utility>

#include "base/functional/callback_helpers.h"
#include "base/metrics/histogram_functions.h"
#include "base/task/single_thread_task_runner.h"
#include "hns/components/sync/service/hns_sync_service_impl.h"
#include "components/sync_device_info/device_info_sync_service.h"
#include "components/sync_device_info/device_info_tracker.h"
#include "components/sync_device_info/local_device_info_provider.h"

namespace syncer {

HnsSyncServiceImplDelegate::HnsSyncServiceImplDelegate(
    DeviceInfoSyncService* device_info_sync_service)
    : device_info_sync_service_(device_info_sync_service),
      weak_ptr_factory_(this) {
  DCHECK(device_info_sync_service_);

  local_device_info_provider_ =
      device_info_sync_service_->GetLocalDeviceInfoProvider();

  device_info_tracker_ = device_info_sync_service_->GetDeviceInfoTracker();
  DCHECK(device_info_tracker_);

  device_info_observer_.Observe(device_info_tracker_);
}

HnsSyncServiceImplDelegate::~HnsSyncServiceImplDelegate() = default;

void HnsSyncServiceImplDelegate::OnDeviceInfoChange() {
  DCHECK(sync_service_impl_);

  RecordP3ASyncStatus();

  const syncer::DeviceInfo* local_device_info =
      local_device_info_provider_->GetLocalDeviceInfo();

  bool found_local_device = false;
  const auto all_devices = device_info_tracker_->GetAllDeviceInfo();
  for (const auto& device : all_devices) {
    if (local_device_info->guid() == device->guid()) {
      found_local_device = true;
      break;
    }
  }

  if (found_local_device && local_device_appeared_callback_) {
    std::move(local_device_appeared_callback_).Run();
  }

  // When our device was removed from the sync chain by some other device,
  // we don't seee it in devices list, we must reset sync in a proper way
  if (!found_local_device) {
    // We can't call OnSelfDeviceInfoDeleted directly because we are on
    // remove device execution path, so posting task
    base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE,
        base::BindOnce(&HnsSyncServiceImplDelegate::OnSelfDeviceInfoDeleted,
                       weak_ptr_factory_.GetWeakPtr()));
  }
}

void HnsSyncServiceImplDelegate::OnSelfDeviceInfoDeleted() {
  sync_service_impl_->OnSelfDeviceInfoDeleted(base::DoNothing());
}

void HnsSyncServiceImplDelegate::SuspendDeviceObserverForOwnReset() {
  device_info_observer_.Reset();
}

void HnsSyncServiceImplDelegate::ResumeDeviceObserver() {
  if (!device_info_observer_.IsObserving()) {
    device_info_observer_.Observe(device_info_tracker_);
  }
}

void HnsSyncServiceImplDelegate::RecordP3ASyncStatus() {
  int num_devices = device_info_tracker_->GetAllDeviceInfo().size();

  // 0 - sync is disabled
  // 1 - one device in chain
  // 2 - two devices in chain
  // 3 - three or more devices in chain
  int p3a_value = std::min(num_devices, 3);

  base::UmaHistogramExactLinear("Hns.Sync.Status.2", p3a_value, 3);
}

void HnsSyncServiceImplDelegate::SetLocalDeviceAppearedCallback(
    base::OnceCallback<void()> local_device_appeared_callback) {
  local_device_appeared_callback_ = std::move(local_device_appeared_callback);
}

}  // namespace syncer
