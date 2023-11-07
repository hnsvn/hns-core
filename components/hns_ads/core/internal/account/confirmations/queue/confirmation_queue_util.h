/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_QUEUE_CONFIRMATION_QUEUE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_QUEUE_CONFIRMATION_QUEUE_UTIL_H_

#include "base/functional/callback_forward.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

struct ConfirmationInfo;

using RebuildConfirmationQueueItemCallback =
    base::OnceCallback<void(const ConfirmationInfo& confirmation)>;

void AddConfirmationQueueItem(const ConfirmationInfo& confirmation);
void RemoveConfirmationQueueItem(const ConfirmationInfo& confirmation);

absl::optional<ConfirmationInfo> MaybeGetNextConfirmationQueueItem();

void RebuildConfirmationQueueItem(
    const ConfirmationInfo& confirmation,
    RebuildConfirmationQueueItemCallback callback);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_QUEUE_CONFIRMATION_QUEUE_UTIL_H_
