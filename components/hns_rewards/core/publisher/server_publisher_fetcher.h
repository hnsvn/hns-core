/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_PUBLISHER_SERVER_PUBLISHER_FETCHER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_PUBLISHER_SERVER_PUBLISHER_FETCHER_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/database/database_server_publisher_info.h"
#include "hns/components/hns_rewards/core/endpoint/private_cdn/private_cdn_server.h"
#include "hns/components/hns_rewards/core/rewards_callbacks.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace publisher {

using FetchCallbackVector =
    std::vector<database::GetServerPublisherInfoCallback>;

// Fetches server publisher info and provides methods for determining
// whether a server publisher info record is expired
class ServerPublisherFetcher {
 public:
  explicit ServerPublisherFetcher(RewardsEngineImpl& engine);

  ServerPublisherFetcher(const ServerPublisherFetcher&) = delete;
  ServerPublisherFetcher& operator=(const ServerPublisherFetcher&) = delete;

  ~ServerPublisherFetcher();

  // Returns a value indicating whether a server info record with
  // the specified last update time is expired
  bool IsExpired(mojom::ServerPublisherInfo* server_info);

  // Fetches server publisher info for the specified publisher key
  void Fetch(const std::string& publisher_key,
             database::GetServerPublisherInfoCallback callback);

  // Purges expired records from the backing database
  void PurgeExpiredRecords();

 private:
  void OnFetchCompleted(const mojom::Result result,
                        mojom::ServerPublisherInfoPtr info,
                        const std::string& publisher_key);

  FetchCallbackVector GetCallbacks(const std::string& publisher_key);

  void RunCallbacks(const std::string& publisher_key,
                    mojom::ServerPublisherInfoPtr server_info);

  const raw_ref<RewardsEngineImpl> engine_;
  std::map<std::string, FetchCallbackVector> callback_map_;
  endpoint::PrivateCDNServer private_cdn_server_;
};

}  // namespace publisher
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_PUBLISHER_SERVER_PUBLISHER_FETCHER_H_
