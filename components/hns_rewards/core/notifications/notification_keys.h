/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_NOTIFICATIONS_NOTIFICATION_KEYS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_NOTIFICATIONS_NOTIFICATION_KEYS_H_

namespace hns_rewards::internal {
namespace notifications {

const char kUpholdBATNotAllowed[] = "uphold_bat_not_allowed";
const char kUpholdInsufficientCapabilities[] =
    "uphold_insufficient_capabilities";
const char kWalletDisconnected[] = "wallet_disconnected";

}  // namespace notifications
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_NOTIFICATIONS_NOTIFICATION_KEYS_H_
