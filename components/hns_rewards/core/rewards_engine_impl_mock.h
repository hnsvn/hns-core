/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_REWARDS_ENGINE_IMPL_MOCK_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_REWARDS_ENGINE_IMPL_MOCK_H_

#include "hns/components/hns_rewards/core/database/database_mock.h"
#include "hns/components/hns_rewards/core/rewards_engine_client_mock.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"
#include "mojo/public/cpp/bindings/associated_receiver.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace hns_rewards::internal {

class AddMockRewardsClient {
 protected:
  AddMockRewardsClient();

  ~AddMockRewardsClient();

  MockRewardsEngineClient mock_client_;
  mojo::AssociatedReceiver<mojom::RewardsEngineClient> mock_client_receiver_{
      &mock_client_};
};

class MockRewardsEngineImpl : private AddMockRewardsClient,
                              public RewardsEngineImpl {
 public:
  MockRewardsEngineImpl();

  ~MockRewardsEngineImpl() override;

  MockRewardsEngineClient* mock_client();

  database::MockDatabase* mock_database();

  MOCK_METHOD1(InitializeDatabase, void(ResultCallback));

  MOCK_METHOD0(database, database::Database*());

 private:
  database::MockDatabase mock_database_{*this};
};

}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_REWARDS_ENGINE_IMPL_MOCK_H_
