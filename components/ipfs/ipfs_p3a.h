/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_IPFS_IPFS_P3A_H_
#define HNS_COMPONENTS_IPFS_IPFS_P3A_H_

#include "base/memory/raw_ptr.h"
#include "base/timer/timer.h"
#include "hns/components/ipfs/ipfs_service_observer.h"
#include "components/prefs/pref_change_registrar.h"

class PrefService;

namespace ipfs {

constexpr char kDetectionPromptCountHistogramName[] =
    "Hns.IPFS.DetectionPromptCount";
constexpr char kGatewaySettingHistogramName[] = "Hns.IPFS.GatewaySetting";
constexpr char kLocalNodeRetentionHistogramName[] =
    "Hns.IPFS.LocalNodeRetention";
constexpr char kDaemonRunTimeHistogramName[] = "Hns.IPFS.DaemonRunTime";

class HnsIpfsClientUpdater;
class IpfsService;

int GetIPFSDetectionPromptBucket(PrefService* prefs);
int GetDaemonUsageBucket(base::TimeDelta elapsed_time);

// Reports IPFS related P3A data.
// Maintains a timer to report in the amount of up time.
class IpfsP3A : public IpfsServiceObserver {
 public:
  IpfsP3A(IpfsService* service,
          PrefService* pref_service);
  ~IpfsP3A() override;
  IpfsP3A(const IpfsP3A&) = delete;
  IpfsP3A& operator=(IpfsP3A&) = delete;
  void Stop();

  // IpfsServiceObserver
  void OnIpfsLaunched(bool result, int64_t pid) override;
  void OnIpfsShutdown() override;

 private:
  void RecordDaemonUsage();
  void RecordInitialIPFSP3AState();
  void FlushTimeDelta();

  void OnIPFSResolveMethodChanged();

  base::RepeatingTimer timer_;
  raw_ptr<IpfsService> service_ = nullptr;
  base::TimeTicks daemon_start_time_;
  base::TimeDelta elapsed_time_;
  const raw_ptr<PrefService> pref_service_;
  PrefChangeRegistrar pref_change_registrar_;
};

}  // namespace ipfs

#endif  // HNS_COMPONENTS_IPFS_IPFS_P3A_H_
