/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_STATS_BROWSER_HNS_STATS_UPDATER_UTIL_H_
#define HNS_COMPONENTS_HNS_STATS_BROWSER_HNS_STATS_UPDATER_UTIL_H_

#include <string>

#include "base/strings/string_piece.h"
#include "base/system/sys_info.h"
#include "base/time/time.h"

namespace hns_stats {

enum class ProcessArch {
  kArchSkip,
  kArchMetal,
  kArchVirt,
};

std::string GetDateAsYMD(const base::Time& time);

std::string GetPlatformIdentifier();

int GetIsoWeekNumber(const base::Time& time);

base::Time GetLastMondayTime(const base::Time& time);

base::Time GetYMDAsDate(const base::StringPiece& ymd);

std::string GetAPIKey();

enum : uint8_t {
  kIsInactiveUser = 0,
  kIsDailyUser = (1 << 0),
  kIsWeeklyUser = (1 << 1),
  kIsMonthlyUser = (1 << 2),
};

uint8_t UsageBitfieldFromTimestamp(const base::Time& last_usage_time,
                                   const base::Time& last_reported_usage_time);

}  // namespace hns_stats

#endif  // HNS_COMPONENTS_HNS_STATS_BROWSER_HNS_STATS_UPDATER_UTIL_H_
