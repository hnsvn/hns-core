/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/sync/engine/hns_syncer_device_poll.h"

#include <memory>

#include "base/functional/callback.h"
#include "base/test/mock_callback.h"
#include "base/test/scoped_mock_clock_override.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace syncer {

TEST(HnsSyncerDevicePoll, ForcedPolling) {
  base::ScopedMockClockOverride clock;
  HnsSyncerDevicePoll hns_syncer_device_poll;
  const auto kDelayBeforeForcedPoll =
      HnsSyncerDevicePoll::GetDelayBeforeForcedPollForTesting();

  base::MockCallback<base::OnceClosure> mock_callback;
  EXPECT_CALL(mock_callback, Run).Times(0);
  hns_syncer_device_poll.CheckIntervalAndPoll(mock_callback.Get());
  testing::Mock::VerifyAndClearExpectations(&mock_callback);

  clock.Advance(kDelayBeforeForcedPoll - base::Seconds(1));
  EXPECT_CALL(mock_callback, Run).Times(0);
  hns_syncer_device_poll.CheckIntervalAndPoll(mock_callback.Get());
  testing::Mock::VerifyAndClearExpectations(&mock_callback);

  clock.Advance(base::Seconds(2));
  EXPECT_CALL(mock_callback, Run).Times(1);
  hns_syncer_device_poll.CheckIntervalAndPoll(mock_callback.Get());
  testing::Mock::VerifyAndClearExpectations(&mock_callback);

  clock.Advance(kDelayBeforeForcedPoll - base::Seconds(2));
  EXPECT_CALL(mock_callback, Run).Times(0);
  hns_syncer_device_poll.CheckIntervalAndPoll(mock_callback.Get());
  testing::Mock::VerifyAndClearExpectations(&mock_callback);

  clock.Advance(base::Seconds(3));
  EXPECT_CALL(mock_callback, Run).Times(1);
  hns_syncer_device_poll.CheckIntervalAndPoll(mock_callback.Get());
  testing::Mock::VerifyAndClearExpectations(&mock_callback);
}

}  // namespace syncer
