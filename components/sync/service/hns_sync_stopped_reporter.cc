/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/sync/service/hns_sync_stopped_reporter.h"

#include <utility>

#include "services/network/public/cpp/shared_url_loader_factory.h"

namespace syncer {

HnsSyncStoppedReporter::HnsSyncStoppedReporter(
    const GURL& sync_service_url,
    const std::string& user_agent,
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory)
    : SyncStoppedReporter(sync_service_url, user_agent, url_loader_factory) {}

HnsSyncStoppedReporter::~HnsSyncStoppedReporter() = default;

void HnsSyncStoppedReporter::ReportSyncStopped(
    const std::string& access_token,
    const std::string& cache_guid,
    const std::string& birthday) {}

}  // namespace syncer
