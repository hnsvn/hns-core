/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/sync/engine/hns_syncer_device_poll.h"
// Forward include to avoid redefine CommitProcessor at upstream header
#include "components/sync/engine/commit_processor.h"

// CommitProcessor is occurred just once in upstream syncer.cc
// so we can use it to redefine and introduce own variable.
#define CommitProcessor                           \
  HnsSyncerDevicePoll hns_syncer_device_poll; \
  CommitProcessor

#define HNS_SYNCER_BUILD_AND_POST_COMMITS_POLLER_CHECK                    \
  hns_syncer_device_poll.CheckIntervalAndPoll(base::BindOnce(             \
      [](Syncer* the_syncer, SyncCycle* cycle, NudgeTracker* nudge_tracker, \
         HnsSyncerDevicePoll* the_hns_syncer_device_poll) {             \
        VLOG(1) << "Forced poll of device info during long commit. Passed " \
                   "time since begin of commit operation is "               \
                << the_hns_syncer_device_poll->SinceBegin();              \
        ModelTypeSet device_only({ModelType::DEVICE_INFO});                 \
        the_syncer->DownloadAndApplyUpdates(                                \
            &device_only, cycle, NormalGetUpdatesDelegate(*nudge_tracker)); \
      },                                                                    \
      this, cycle, nudge_tracker, &hns_syncer_device_poll));

#include "src/components/sync/engine/syncer.cc"

#undef HNS_SYNCER_BUILD_AND_POST_COMMITS_POLLER_CHECK
#undef CommitProcessor
