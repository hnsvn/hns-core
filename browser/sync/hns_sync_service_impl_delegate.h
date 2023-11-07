/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_SYNC_HNS_SYNC_SERVICE_IMPL_DELEGATE_H_
#define HNS_BROWSER_SYNC_HNS_SYNC_SERVICE_IMPL_DELEGATE_H_

#include "hns/components/sync/service/sync_service_impl_delegate.h"

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "components/sync_device_info/device_info_tracker.h"

class Profile;

namespace syncer {

class HnsSyncServiceImpl;
class DeviceInfoSyncService;
class DeviceInfoTracker;
class LocalDeviceInfoProvider;

// Helper class to prevent pass of profile pointer into HnsSyncServiceImpl
// and to keep DeviceInfoSyncService
class HnsSyncServiceImplDelegate
    : public SyncServiceImplDelegate,
      public syncer::DeviceInfoTracker::Observer {
 public:
  explicit HnsSyncServiceImplDelegate(
      DeviceInfoSyncService* device_info_sync_service);
  ~HnsSyncServiceImplDelegate() override;

  void SuspendDeviceObserverForOwnReset() override;
  void ResumeDeviceObserver() override;

  void SetLocalDeviceAppearedCallback(
      base::OnceCallback<void()> local_device_appeared_callback) override;

 private:
  // syncer::DeviceInfoTracker::Observer:
  void OnDeviceInfoChange() override;

  void OnSelfDeviceInfoDeleted(void);

  void RecordP3ASyncStatus();

  raw_ptr<syncer::DeviceInfoTracker> device_info_tracker_ = nullptr;
  raw_ptr<syncer::LocalDeviceInfoProvider> local_device_info_provider_ =
      nullptr;
  base::ScopedObservation<syncer::DeviceInfoTracker,
                          syncer::DeviceInfoTracker::Observer>
      device_info_observer_{this};

  raw_ptr<DeviceInfoSyncService> device_info_sync_service_ = nullptr;

  // This is triggered once after SetLocalDeviceAppearedCallback
  // when the local device first appears in the changed synced devices list
  base::OnceCallback<void()> local_device_appeared_callback_;

  base::WeakPtrFactory<HnsSyncServiceImplDelegate> weak_ptr_factory_;

  HnsSyncServiceImplDelegate(const HnsSyncServiceImplDelegate&) = delete;
  HnsSyncServiceImplDelegate& operator=(const HnsSyncServiceImplDelegate&) =
      delete;
};

}  // namespace syncer

#endif  // HNS_BROWSER_SYNC_HNS_SYNC_SERVICE_IMPL_DELEGATE_H_
