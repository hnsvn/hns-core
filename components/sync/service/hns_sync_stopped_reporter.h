/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_SYNC_SERVICE_HNS_SYNC_STOPPED_REPORTER_H_
#define HNS_COMPONENTS_SYNC_SERVICE_HNS_SYNC_STOPPED_REPORTER_H_

#include <string>

#include "components/sync/service/sync_stopped_reporter.h"

namespace syncer {

class HnsSyncStoppedReporter : public SyncStoppedReporter {
 public:
  HnsSyncStoppedReporter(
      const GURL& sync_service_url,
      const std::string& user_agent,
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory);
  ~HnsSyncStoppedReporter() override;

  void ReportSyncStopped(const std::string& access_token,
                         const std::string& cache_guid,
                         const std::string& birthday) override;
};

}  // namespace syncer

#endif  // HNS_COMPONENTS_SYNC_SERVICE_HNS_SYNC_STOPPED_REPORTER_H_
