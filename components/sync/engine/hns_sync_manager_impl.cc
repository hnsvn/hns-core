/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/sync/engine/hns_sync_manager_impl.h"

#include <utility>

#include "components/sync/engine/sync_scheduler.h"
#include "components/sync/protocol/sync_protocol_error.h"

namespace syncer {

HnsSyncManagerImpl::HnsSyncManagerImpl(
    const std::string& name,
    network::NetworkConnectionTracker* network_connection_tracker)
    : SyncManagerImpl(name, network_connection_tracker) {}

HnsSyncManagerImpl::~HnsSyncManagerImpl() = default;

void HnsSyncManagerImpl::StartSyncingNormally(base::Time last_poll_time) {
  SyncManagerImpl::StartSyncingNormally(last_poll_time);
  // Remove this hack when we have FCM invalidation integrated.
  RefreshTypes(ModelTypeSet::All());
}

void HnsSyncManagerImpl::PermanentlyDeleteAccount(
    base::OnceCallback<void(const SyncProtocolError&)> callback) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  scheduler_->SchedulePermanentlyDeleteAccount(std::move(callback));
}

}  // namespace syncer
