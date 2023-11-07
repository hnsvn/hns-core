/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_CONTRIBUTION_CONTRIBUTION_UTIL_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_CONTRIBUTION_CONTRIBUTION_UTIL_H_

#include <map>
#include <string>

#include "hns/components/hns_rewards/core/mojom_structs.h"

namespace hns_rewards::internal {
namespace contribution {

mojom::ReportType GetReportTypeFromRewardsType(const mojom::RewardsType type);

mojom::ContributionProcessor GetProcessor(const std::string& wallet_type);

std::string GetNextProcessor(const std::string& current_processor);

bool HaveEnoughFundsToContribute(double* amount,
                                 const bool partial,
                                 const double balance);

int32_t GetVotesFromAmount(const double amount);

}  // namespace contribution
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_CONTRIBUTION_CONTRIBUTION_UTIL_H_
