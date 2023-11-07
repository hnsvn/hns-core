/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/timer/timer.h"

#include <utility>

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/common/random/random_util.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

namespace {
absl::optional<base::TimeDelta> g_timer_delay_for_testing;
}  // namespace

Timer::Timer() = default;

Timer::~Timer() {
  Stop();
}

base::Time Timer::Start(const base::Location& location,
                        const base::TimeDelta delay,
                        base::OnceClosure user_task) {
  Stop();

  const base::Time fire_at =
      base::Time::Now() + g_timer_delay_for_testing.value_or(delay);
  timer_.Start(location, fire_at, std::move(user_task));
  return fire_at;
}

base::Time Timer::StartWithPrivacy(const base::Location& location,
                                   const base::TimeDelta delay,
                                   base::OnceClosure user_task) {
  base::TimeDelta rand_delay = RandTimeDelta(delay);
  if (rand_delay.is_negative()) {
    rand_delay = base::Seconds(1);
  }

  return Start(location, rand_delay, std::move(user_task));
}

bool Timer::IsRunning() const {
  return timer_.IsRunning();
}

bool Timer::Stop() {
  if (!IsRunning()) {
    return false;
  }

  timer_.Stop();

  return true;
}

ScopedTimerDelaySetterForTesting::ScopedTimerDelaySetterForTesting(
    const base::TimeDelta delay) {
  g_timer_delay_for_testing = delay;
}

ScopedTimerDelaySetterForTesting::~ScopedTimerDelaySetterForTesting() {
  g_timer_delay_for_testing = absl::nullopt;
}

}  // namespace hns_ads
