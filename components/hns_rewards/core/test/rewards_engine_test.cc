/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/test/rewards_engine_test.h"
#include "hns/components/hns_rewards/common/mojom/rewards_engine.mojom-test-utils.h"

namespace hns_rewards::internal {

RewardsEngineTest::RewardsEngineTest()
    : engine_(client_receiver_.BindNewEndpointAndPassDedicatedRemote()) {}

RewardsEngineTest::~RewardsEngineTest() = default;

void RewardsEngineTest::InitializeEngine() {
  const auto result = mojom::RewardsEngineAsyncWaiter(&engine_).Initialize();
  DCHECK(result == mojom::Result::OK);
}

void RewardsEngineTest::AddNetworkResultForTesting(
    const std::string& url,
    mojom::UrlMethod method,
    mojom::UrlResponsePtr response) {
  DCHECK(response);
  client_.AddNetworkResultForTesting(url, method, std::move(response));
}

void RewardsEngineTest::SetLogCallbackForTesting(
    TestRewardsEngineClient::LogCallback callback) {
  client_.SetLogCallbackForTesting(callback);
}

}  // namespace hns_rewards::internal
