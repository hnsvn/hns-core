/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_SYNC_ENGINE_HNS_SYNCER_DEVICE_POLL_H_
#define HNS_COMPONENTS_SYNC_ENGINE_HNS_SYNCER_DEVICE_POLL_H_

#include "base/functional/callback_forward.h"
#include "base/gtest_prod_util.h"
#include "base/time/time.h"

namespace syncer {

FORWARD_DECLARE_TEST(HnsSyncerDevicePoll, ForcedPolling);

class HnsSyncerDevicePoll {
 public:
  HnsSyncerDevicePoll(const HnsSyncerDevicePoll&) = delete;
  HnsSyncerDevicePoll& operator=(const HnsSyncerDevicePoll&) = delete;
  HnsSyncerDevicePoll();

  void CheckIntervalAndPoll(base::OnceClosure poll_devices);
  base::TimeDelta SinceBegin();

 private:
  FRIEND_TEST_ALL_PREFIXES(HnsSyncerDevicePoll, ForcedPolling);
  static base::TimeDelta GetDelayBeforeForcedPollForTesting();

  const base::TimeTicks ticks_at_begin_;
  base::TimeTicks ticks_;
};

}  // namespace syncer

#endif  // HNS_COMPONENTS_SYNC_ENGINE_HNS_SYNCER_DEVICE_POLL_H_
